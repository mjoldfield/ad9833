import gpiozero

class AD9833:

    def __init__(self, data, clk, fsync):
        self.dataPin  = gpiozero.OutputDevice(pin = data)
        self.clkPin   = gpiozero.OutputDevice(pin = clk)
        self.fsyncPin = gpiozero.OutputDevice(pin = fsync)

        self.fsyncPin.on()
        self.clkPin.on()
        self.dataPin.off()

        self.clk_freq = 25.0e6

    def set_freq(self, f):
        flag_b28  = 1 << 13
        flag_freq = 1 << 14

        scale = 1 << 28
        n_reg = int(f * scale / self.clk_freq)

        n_low = n_reg         & 0x3fff
        n_hi  = (n_reg >> 14) & 0x3fff

        self.send16(flag_b28)
        self.send16(flag_freq | n_low)
        self.send16(flag_freq | n_hi)

    def send16(self, n):
        self.fsyncPin.off()

        mask = 1 << 15
        for i in range(0, 16):

            self.dataPin.value = bool(n & mask)
            self.clkPin.off()
            self.clkPin.on()

            mask = mask >> 1

        self.dataPin.off()
        self.fsyncPin.on()

ad = AD9833(10, 11, 8)
while 1:
    for f in range(10,10000):
        ad.set_freq(f)

