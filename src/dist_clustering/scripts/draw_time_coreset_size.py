# draw bars for time when coreset_size changes

import numpy as np
import matplotlib.pyplot as plt



fontsize = 20
plt.rc('ytick',labelsize=fontsize)
plt.rc('xtick',labelsize=fontsize)


fig = plt.figure()
ax = fig.add_subplot(111)


ys1 = np.array([0.15, 0.26, 0.53, 0.69])
name1 = 'ball-grow'
ys2 = np.array([0.26, 0.48, 0.87, 1.25])
name2 = 'rand'
ys3 = np.array([0.72, 1.46, 3.0, 4.4])
name3 = 'k-means++'
ys4 = np.array([8.8, 24.8, 50.9, 76.6])
name4 = 'k-means||'

N = len(ys1)

## necessary variables
ind = np.arange(N)                # the x locations for the groups
width = 0.2                      # the width of the bars
x_label = 'coreset size'
y_label = 'log(running time/msecs)'
title = 'xxxx'
xTickMarks = [r'$\approx 5000$', r'$\approx 10000$', r'$\approx 20000$', r'$\approx 30000$']

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
ax.set_ylim(2, 7)
ax.set_xlabel(x_label, fontsize=fontsize)
ax.set_ylabel(y_label, fontsize=fontsize)
# ax.set_title(title)
ax.set_xticks(ind + 1.5 * width)
xtickNames = ax.set_xticklabels(xTickMarks)
plt.setp(xtickNames, rotation=0)

## add a legend
if not block_kmeans_par:
    ax.legend( (rects1[0], rects2[0], rects3[0], rects4[0]), (name1, name2, name3, name4), fontsize=fontsize, loc=2 )
else:
    ax.legend( (rects1[0], rects2[0], rects3[0]), (name1, name2, name3), fontsize=fontsize, loc=2 )

plt.show()
