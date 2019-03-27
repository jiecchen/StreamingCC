import streamingcc._hyper_loglog as _hyper_loglog

hyper_loglog = _hyper_loglog.Pyhyper_loglog


class HL:

    def __init__(self):
        self.hl = hyper_loglog()

    def update(self, x):
        self.hl.Update(x)

    def estimate(self):
        return self.hl.Estimate()
