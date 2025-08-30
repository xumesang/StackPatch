from coapthon.client.helperclient import HelperClient
import time

host = "192.168.1.18"
port = 5683
path ="test"

# This test method got quite long latency, there must be some problem

client = HelperClient(server=(host, port))

for i in range(5000):
    tic = time.perf_counter()
    response = client.get(path)
    toc = time.perf_counter()
    print(toc - tic)

# print(response.pretty_print())
# client.stop()