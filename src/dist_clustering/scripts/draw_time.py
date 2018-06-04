# draw bars

import numpy as np
import matplotlib.pyplot as plt

fontsize = 20
plt.rc('ytick',labelsize=fontsize)
plt.rc('xtick',labelsize=fontsize)


fig = plt.figure()
ax = fig.add_subplot(111)


ys1 = np.array([12.077, 3.182, 1.644, 1.08])
name1 = 'ball-grow'
ys2 = np.array([24.069, 5.952, 2.844, 1.85])
name2 = 'rand'
ys3 = np.array([73.302, 19, 11, 7.4])
name3 = 'k-means++'
ys4 = np.array([263.898, 149.83, 127.04, 100.285])
name4 = 'k-means||'

N = len(ys1)

## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.2                      # the width of the bars
x_label = '# of sites'
y_label = 'log(running time/msecs)'
title = 'xxxx'
xTickMarks = ['5', '10', '15', '20']

block_kmeans_par = False


## the bars
rects1 = ax.bar(ind, np.log10(1000 * ys1), width,
                color='green',
                hatch='/')


rects2 = ax.bar(ind + width, np.log10(1000 * ys2), width,
                color='orange',
                hatch='\\')

rects3 = ax.bar(ind + 2 * width, np.log10(1000 * ys3), width,
                color='red',
                hatch='=')


if not block_kmeans_par:
    rects4 = ax.bar(ind + 3 * width, np.log10(1000 * ys4), width,
                    color='blue',
                    hatch='+')
    
# axes and labels
ax.set_xlim(-width, len(ind))
ax.set_ylim(3, 6.5)
ax.set_xlabel(x_label, fontsize=fontsize)
ax.set_ylabel(y_label, fontsize=fontsize)
# ax.set_title(title)
ax.set_xticks(ind + width)
xtickNames = ax.set_xticklabels(xTickMarks)
plt.setp(xtickNames, rotation=0)

## add a legend
if not block_kmeans_par:
    ax.legend( (rects1[0], rects2[0], rects3[0], rects4[0]), (name1, name2, name3, name4), fontsize=fontsize )
else:
    ax.legend( (rects1[0], rects2[0], rects3[0]), (name1, name2, name3), fontsize=fontsize )

plt.show()
