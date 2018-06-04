# draw bars

import numpy as np
import matplotlib.pyplot as plt



fig = plt.figure()
ax = fig.add_subplot(111)

## the data


ys1 = [12.077, 3.082, 1.444, 1.28]
name1 = 'bg'
ys2 = [483.898, 311.983, 271.704, 260.285]
name2 = 'k-means||'
ys3 = [24.069, 5.952, 2.844, 1.85]
name3 = 'rand'

N = len(ys1)


## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.2                      # the width of the bars
x_label = '# of partitions'
y_label = 'running time/sec'
title = 'xxxx'
xTickMarks = ['5', '10', '15', '20']



## the bars
rects1 = ax.bar(ind, ys1, width,
                hatch='/')


rects2 = ax.bar(ind + width, ys2, width,
                hatch='\\')


rects3 = ax.bar(ind + 2 * width, ys3, width,
                hatch='-')

# axes and labels
ax.set_xlim(-2 * width, len(ind))
# ax.set_ylim(0, 45)
ax.set_xlabel(x_label)
ax.set_ylabel(y_label)
# ax.set_title(title)
ax.set_xticks(ind + width)
xtickNames = ax.set_xticklabels(xTickMarks)
plt.setp(xtickNames, rotation=0, fontsize=10)

## add a legend
ax.legend( (rects1[0], rects2[0], rects3[0]), (name1, name2, name3) )

plt.show()
