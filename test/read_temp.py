from w1thermsensor import W1ThermSensor
import time


sensor = W1ThermSensor()

time.sleep(2)
temperature = sensor.get_temperature()
print("Temperature: {:.2f}°C".format(temperature))
