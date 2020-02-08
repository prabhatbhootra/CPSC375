import math

nArray = [10, 20, 40, 80, 160, 320]
pArray = [1, 2, 4, 8, 16, 32, 64, 128]

print("Holding n constant, while changing p")
for eachn in nArray:
    n = eachn
    for eachp in pArray:
        p = eachp
        speedup = (n*n)/((n*n)/p + math.log2(p))
        efficiency = (n*n)/(p*((n*n)/p + math.log2(p)))
        print("n is " + str(n) + ", p is " + str(p) + ", speedup is " + str(speedup) + ", efficiency is " + str(efficiency))
print("")
print("Holding p constant, while changing n")
for eachp in pArray:
    p = eachp
    for eachn in nArray:
        n = eachn
        speedup = (n*n)/((n*n)/p + math.log2(p))
        efficiency = (n*n)/(p*((n*n)/p + math.log2(p)))
        print("n is " + str(n) + ", p is " + str(p) + ", speedup is " + str(speedup) + ", efficiency is " + str(efficiency))
        

