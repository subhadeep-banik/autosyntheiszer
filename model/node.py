import datetime as dt
from .enums import *
from marshmallow import Schema, fields


class Node(object):
    def __init__(self, nodeid, nodetype,  powermax, powermin, powerbase, trust,proctype,threshold):
        self.nodeid = nodeid
        self.nodetype = nodetype 
 
        self.powermax = powermax
        self.powermin = powermin
        self.powerbase = powerbase
        self.trust = trust
 
        self.proctype= proctype
        self.threshold= threshold


    def __repr__(self):
        return '<Node(name={self.nodetype!r})>'.format(self=self)

    def show(self):
        print("NodeID:", self.nodeid)
        print("  Nodetype:", self.nodetype)
        print("  Processor type:", self.proctype)
        print("  powermax:", self.powermax)
        print("  powermin:", self.powermin)
        print("  powebase:", self.powermin) 
        print("  trust level:", self.trust)
        print("  High CPU Threshold:", self.threshold)
 
        
class NodeSchema(Schema):
    nodeid = fields.Str()
 
 
    powermax = fields.Float()
    powermin = fields.Float()
    powerbase = fields.Float()        
    trust = fields.Enum(Level, by_value=True)
    nodetype = fields.Enum(NodeType, by_value=True)       
    proctype= fields.Enum(ProcessorType, by_value=True)    
    threshold= fields.Float()  



class MirtoPrimitiveSchema(Schema):
    primitive_type = fields.Enum(PrimitiveType, by_value=True)
    name = fields.Str()
    security_level = fields.Enum(Level, by_value=True)
    certification = fields.Enum(CertificationType, by_value=True)
    energy_consumption = fields.Enum(Level, by_value=True)
    throughput = fields.Enum(Level, by_value=True)
    commandline=fields.Dict(keys=fields.Str(), values=fields.Str()) 
