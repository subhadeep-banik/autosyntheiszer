from enum import Enum

# subclass str as well so it can be (de)serialized by JSON
class NodeType(str,Enum):
    CLU = "MYRTUS-Mobility.CloudInstance"
    FOG = "MYRTUS-Mobility.FogNode"
    CAM = "MYRTUS-mobility.EdgeCamera"
    DEF = "Default"
    


class ProcessorType(str,Enum):
    XEP = "Intel Xeon Platinum 8358, 32 cores @ 2.60 GHz"
    XEE = "Intel Xeon E-2224, 4 cores @ 3.40 GHz"
    ARM = "ARM Cortex-A73, Quad-core 1.4 GHz"
    NVD = "NVIDIA Jetson TX2"
    DEF = "Default"
  

class Level(str,Enum):
    LOW = "low"
    MEDIUM = "medium"
    HIGH = "high"

class PrimitiveType(str,Enum):
    ENCRYPTION = "encryption"
    HASHING = "hashing"
    SIGNATURE = "signature"
    KEYEXCHANGE = "keyexchange"
    PRIVACY = "privacy"

class CertificationType(str, Enum): # Just example of certifications
    NONE = "none"
    FIPS140_3 = "fips 140-3"
    ISO27001_2022 = "iso 27001:2022"
