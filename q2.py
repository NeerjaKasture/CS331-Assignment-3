
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import OVSSwitch, OVSController
from mininet.link import TCLink
from mininet.cli import CLI
from mininet.log import setLogLevel

class CustomTopo(Topo):
    def build(self):
        # Switches (STP enabled)
        s1 = self.addSwitch('s1', cls=OVSSwitch, stp=True)
        s2 = self.addSwitch('s2', cls=OVSSwitch, stp=True)
        s3 = self.addSwitch('s3', cls=OVSSwitch, stp=True)
        s4 = self.addSwitch('s4', cls=OVSSwitch, stp=True)

        # Private network hosts
        h1 = self.addHost('h1', ip='10.1.1.2/24')
        h2 = self.addHost('h2', ip='10.1.1.3/24')

        # Public network hosts
        for i in range(3, 9):
            self.addHost(f'h{i}', ip=f'10.0.0.{i+1}/24')

        # NAT router
        h9 = self.addHost('h9')

        # Host-switch connections
        self.addLink('h3', s2, cls=TCLink, delay='5ms')
        self.addLink('h4', s2, cls=TCLink, delay='5ms')
        self.addLink('h5', s3, cls=TCLink, delay='5ms')
        self.addLink('h6', s3, cls=TCLink, delay='5ms')
        self.addLink('h7', s4, cls=TCLink, delay='5ms')
        self.addLink('h8', s4, cls=TCLink, delay='5ms')

        # Switch interconnections (simplified to avoid STP blocking)
        self.addLink(s1, s2, cls=TCLink, delay='7ms')
        self.addLink(s2, s3, cls=TCLink, delay='7ms')
        self.addLink(s3, s4, cls=TCLink, delay='7ms')
        self.addLink(s4, s1, cls=TCLink, delay='7ms')

        # NAT connections
        self.addLink(h9, s1, cls=TCLink, delay='5ms')  # Public interface
        self.addLink(h1, h9, cls=TCLink, delay='5ms')  # Private interface 1
        self.addLink(h2, h9, cls=TCLink, delay='5ms')  # Private interface 2

def run():
    topo = CustomTopo()
    net = Mininet(topo=topo, link=TCLink, controller=OVSController)
    net.start()

    h1, h5, h9 = net.get('h1', 'h5', 'h9')

    # Configure h9 interfaces
    h9.cmd('ifconfig h9-eth0 10.0.0.10/24')  # Public interface
    h9.cmd('ifconfig h9-eth1 10.1.1.1/24')   # Private interface to h1
    h9.cmd('ifconfig h9-eth2 10.1.1.4/24')   # Private interface to h2

    # Routing configuration
    h1.cmd('ip route add default via 10.1.1.1')
    h5.cmd('ip route add 10.1.1.0/24 via 10.0.0.10')

    # Enable IP forwarding and NAT on h9
    h9.cmd('sysctl -w net.ipv4.ip_forward=1')
    h9.cmd('iptables -t nat -F')  # Clear existing NAT rules
    h9.cmd('iptables -t nat -A POSTROUTING -s 10.1.1.0/24 -o h9-eth0 -j MASQUERADE')
    
    # Allow ICMP forwarding and bidirectional traffic on h9
    h9.cmd('iptables -A FORWARD -p icmp -j ACCEPT')
    h9.cmd('iptables -A FORWARD -i h9-eth0 -o h9-eth1 -m state --state RELATED,ESTABLISHED -j ACCEPT')
    h9.cmd('iptables -A FORWARD -i h9-eth1 -o h9-eth0 -j ACCEPT')

    # Add DNAT rule for incoming traffic to private network (optional for testing)
    h9.cmd('iptables -t nat -A PREROUTING -d 10.0.0.10 -j DNAT --to-destination 10.1.1.2')

    # Static ARP entries for bidirectional communication
    mac_h9_pub = h9.cmd("cat /sys/class/net/h9-eth0/address").strip()
    mac_h9_priv = h9.cmd("cat /sys/class/net/h9-eth1/address").strip()
    
    h1.cmd(f"arp -s 10.1.1.1 {mac_h9_priv}")
    h5.cmd(f"arp -s 10.0.0.10 {mac_h9_pub}")
    h9.cmd(f"arp -s 10.0.0.6 {h5.MAC()}")  # Missing entry for `h5`

    CLI(net)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    run()
