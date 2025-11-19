import datetime as dt
from marshmallow import Schema, fields
import yaml
from  model.enums import *
from  model.node import *

with open("SafeMobilityTopologyTemplate.tosca", 'r') as f:
    cfg = yaml.safe_load(f)
nodes=cfg['topology_template']['groups']['compute_nodes']['node_templates']
softs=cfg['topology_template']['groups']['software_component_nodes']['node_templates']
L=[]
print("Network Elements") 
count = 1    
for k,v in nodes.items():
    print(k,v['type'])
    s=v['properties']
    if 'processingCapabilities' in s.keys():
        s['processingCapability'] = s.pop('processingCapabilities')
    #if 'processingCapability' in s.keys():
        #print (s['processingCapability'])

    if 'maxPowerConsumptionThreshold' in s.keys():
        maxp= (float(s['maxPowerConsumptionThreshold'].split("W")[0]))
    else:
        maxp= s['maxPowerConsumptionThreshold'] = 0
    if 'minPowerConsumptionThreshold' in s.keys():
        minp = (float(s['minPowerConsumptionThreshold'].split("W")[0]))
    else:
        minp = s['minPowerConsumptionThreshold']=0

    if 'baselineAveragePowerConsumption' in s.keys():
        avp= (float(s['baselineAveragePowerConsumption'].split("W")[0]))
    else:
        avp = s['baselineAveragePowerConsumption']=0
    thr = float(s['highCpuThreshold'])


    match (v['type']):
        case "MYRTUS-Mobility.CloudInstance":
            ctype= NodeType.CLU
        case "MYRTUS-Mobility.FogNode":
            ctype= NodeType.FOG
        case "MYRTUS-mobility.EdgeCamera":
            ctype= NodeType.CAM
        case _:
            ctype= NodeType.DEF
      
    
    match (s['processingCapability']):
        case "Intel Xeon Platinum 8358, 32 cores @ 2.60 GHz":
            ptype= ProcessorType.XEP
        case "Intel Xeon E-2224, 4 cores @ 3.40 GHz":
            ptype= ProcessorType.XEE
        case "ARM Cortex-A73, Quad-core 1.4 GHz":
            ptype= ProcessorType.ARM
        case "NVIDIA Jetson TX2":
            ptype= ProcessorType.NVD
        case _:
            ptype= ProcessorType.DEF
    
    u = Node(count , ctype,  maxp, minp, avp, Level.HIGH, ptype ,  thr)
    count=count+1
    L.append(u)
for device in L:
    device.show()
for device in L:
    for sec in list(Level):
        print("docker build -t encryption -d [device.proctype] -s seclevel -tp tplevel", device.nodeid, device.nodetype, device.proctype, sec)
