import m5
from m5.objects import *
from cache import *

######################################
from optparse import OptionParser

parser = OptionParser()
parser.add_option("--clk",help="CPU clk. Default: 1GHz")
parser.add_option("--cache_block",help="CPU cache block. Default: 64")
parser.add_option("--l1i_size",help="L1 instruction cache size. Default: 16kB.")
parser.add_option("--l1d_size",help="L1 data cache size. Default: Default: 64kB.")
parser.add_option("--l2_size",help="L2 cache size. Default: 256kB.")
parser.add_option("--mem_size",help="Memory size. Default: 512MB.")
parser.add_option("--test_dir",help="Test directory. Default: tests/test-progs/hello/bin/x86/linux/hello")
parser.add_option("--ddr4",help="DDR4 with 2400Mhz. Default:DDR3 1600Mhz")

(options,args) = parser.parse_args()
####################################

#the system object will be the father of other objects
system = System()

#we donnot care about the voltage_domain,so just use the default value
system.clk_domain = SrcClockDomain()
if not options or not options.clk:
    system.clk_domain.clock = '1GHz'
else:
    system.clk_domain.clock = options.clk
system.clk_domain.voltage_domain = VoltageDomain()

#use timing mode for memory simulation,we will almost always use this
system.mem_mode = 'timing'
if not options or not options.mem_size:
    system.mem_ranges = [AddrRange('512MB')]
else:
    system.mem_ranges = [AddrRange(options.mem_size)]

#use TimingSimpleCPU which excute every instruction in a single cycle
system.cpu = TimingSimpleCPU()

#creating the L1 caches
system.cpu.icache = L1ICache(options)
system.cpu.dcache = L1DCache(options)

#then connect the ports
system.cpu.icache.connectCPU(system.cpu)
system.cpu.dcache.connectCPU(system.cpu)

#we have to create an L2 bus to connect L1 cache to L2 cache
system.l2bus = L2XBar()
system.cpu.icache.connectBus(system.l2bus)
system.cpu.dcache.connectBus(system.l2bus)

#next we can create L2 cache and connect it to memory bus
system.l2cache = L2Cache(options)
system.l2cache.connectCPUSideBus(system.l2bus)
system.membus = SystemXBar()
system.l2cache.connectMemSideBus(system.membus)

#define the cache line
if not options or not options.cache_block:
    system.cache_line_size = 64
else:
    system.cache_line_size = options.cache_block

#these four lines are to make sure that our system will function correctly
#connecting the PIO and interrupt ports to the memory bus is an x86-specific requirement
system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

system.system_port = system.membus.cpu_side_ports

#we need a memory controller and connect it to the bus,here we use DDR3 controller
#the memory range will be responsible for the entire memory range of our system
system.mem_ctrl = MemCtrl()
if not options or not options.ddr4:
    system.mem_ctrl.dram = DDR3_1600_8x8()
else:
    system.mem_ctrl.dram = DDR4_2400_16x4()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

#now we have finished instantiating our simulated system
if not options or not options.test_dir:
    binary = 'tests/test-progs/hello/bin/x86/linux/hello'
else:
    binary = options.test_dir
#let us set the CPU workloadi
system.workload = SEWorkload.init_compatible(binary)

#we have to create the process to run out CPU
process = Process()
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

root = Root(full_system = False, system = system)
m5.instantiate()

print('Beginning simulation!')
exit_event = m5.simulate()

print('Exiting @ tick {} because {}'
        .format(m5.curTick(),exit_event.getCause()))
