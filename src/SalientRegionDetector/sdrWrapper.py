from ctypes import cdll, c_char_p, c_bool
sdrs = cdll.LoadLibrary("./saliency.so")

class SDRWrapper():
    """Wrapper for SalientRegionDetector"""
    def __init__(self):
        self.obj = sdrs.SalientRegionDetector_New()
    
    def saliency(self, pic, doSegmentation = True):
        """Detect saliency from the picture, and optionally do mean shift based
        processing to extract the most salient object.      
        """
        cpic = c_char_p(pic)
        cseg = c_bool(doSegmentation)
        print sdrs.saliency(self.obj, cpic, cseg)
        

if __name__ == "__main__":
    sdrwrap = SDRWrapper()
    sdrwrap.saliency("../../pics/bird.jpg", False)