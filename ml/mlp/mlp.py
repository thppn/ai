import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('train.txt')

x = data[:, 0]
y = data[:, 1]
categories = data[:, 2]

cmap = plt.get_cmap('tab10', len(np.unique(categories)))

plt.scatter(x, y, c=categories, cmap=cmap, marker='o', edgecolor='k', linewidth=0.5)
plt.xlabel('X-axis label')
plt.ylabel('Y-axis label')
plt.title('Scatter Plot of X-Y Points with Categorical Colors')
plt.colorbar(ticks=np.unique(categories), label='Category')

plt.savefig('train2.png')
