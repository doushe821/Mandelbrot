import matplotlib.pyplot as plt

with open('data.dat', 'r') as f:
   data = [float(line.strip()) for line in f]


plt.hist(data[1:5], bins = 20, color = 'blue', edgecolor = 'black', alpha = 0.7)
plt.grid(axis='y', linestyle='--')
plt.title('Latency distribution (raw)')
plt.xlabel('Latency')
plt.ylabel('Quantity')
plt.savefig('histRaw.png', dpi = 300)