# draw bars

import numpy as np
import matplotlib.pyplot as plt

fontsize = 20
plt.rc('ytick',labelsize=fontsize)
plt.rc('xtick',labelsize=fontsize)


fig = plt.figure()
ax = fig.add_subplot(111)

## the data

scale = 1.

ys1 = np.array([49718, 49709, 49723, 49698]) / scale
name1 = 'ball-grow'
ys2 = np.array([49700, 49700, 49700, 49700]) / scale
name2 = 'rand'
ys3 = np.array([49718, 49709, 49723, 49698]) / scale
name3 = 'k-means++'
ys4 = np.array([i * j for i, j in zip(ys1, [5, 10, 15, 20])] ) / scale
name4 = 'k-means||'


N = len(ys1)


## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.2                      # the width of the bars
x_label = '# of sites'
y_label = 'communication'
title = 'communication'
xTickMarks = ['5', '10', '15', '20']

block_kmeans_par = False




## the bars
rects1 = ax.bar(ind, ys1, width,
                color='green',
                hatch='/')


rects2 = ax.bar(ind + width, ys2, width,
                color='orange',
                hatch='\\')

rects3 = ax.bar(ind + 2 * width, ys3, width,
                color='red',
                hatch='=')

if not block_kmeans_par:
    rects4 = ax.bar(ind + 3 * width, ys4, width,
                    color='blue',
                    hatch='+')

# axes and labels
ax.set_xlim(-width, len(ind))
ax.set_ylim(0, )
ax.set_xlabel(x_label, fontsize=fontsize)
ax.set_ylabel(y_label, fontsize=fontsize)
# ax.set_title(title)
ax.set_xticks(ind + width)
xtickNames = ax.set_xticklabels(xTickMarks)
plt.setp(xtickNames, rotation=0)
ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
## add a legend
if not block_kmeans_par:
    ax.legend( (rects1[0], rects2[0], rects3[0], rects4[0]), (name1, name2, name3, name4), loc=2, fontsize=fontsize )
else:
    ax.legend( (rects1[0], rects2[0], rects3[0]), (name1, name2, name3))

plt.show()
