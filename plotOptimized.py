import matplotlib.pyplot as plt

with open('data.dat', 'r') as f:
   data = [float(line.strip()) for line in f]


plt.hist(data[6:10], bins = 20, color = 'blue', edgecolor = 'black', alpha = 0.7)
plt.grid(axis='y', linestyle='--')
plt.title('Latency distribution (optimized)')
plt.xlabel('Latency')
plt.ylabel('Quantity')
plt.savefig('histOptimized.png', dpi = 300)