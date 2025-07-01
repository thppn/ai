import matplotlib.pyplot as plt
import numpy as np

# Load data from a file
data = np.loadtxt('train.txt')

# Extract x, y, and color values
x = data[:, 0]
y = data[:, 1]
categories = data[:, 2]

# Define a colormap for discrete categories
cmap = plt.get_cmap('tab10', len(np.unique(categories)))

# Plot the points with different colors for each category
plt.scatter(x, y, c=categories, cmap=cmap, marker='o', edgecolor='k', linewidth=0.5)
plt.xlabel('X-axis label')
plt.ylabel('Y-axis label')
plt.title('Scatter Plot of X-Y Points with Categorical Colors')
plt.colorbar(ticks=np.unique(categories), label='Category')

# Save the figure to an image file
plt.savefig('train2.png')
