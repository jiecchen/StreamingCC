StreamingCC
----------------------
A C++ library for summarizing data (data streams, in particular).

[![TravisCI](https://travis-ci.org/jiecchen/StreamingCC.svg?branch=master)](https://travis-ci.org/jiecchen/StreamingCC)
[![DUB](https://img.shields.io/dub/l/vibe-d.svg)]()

## Algorithms
`StreamingCC` implements various [streaming algorithms](https://en.wikipedia.org/wiki/Streaming_algorithm) and [probabilistic data structures](https://en.wikipedia.org/wiki/Category:Probabilistic_data_structures). They can be used to effectively summarize the data stream even when data is too large to fit into memory.

Algorithms/Data Structures (will be) included in `StreamingCC`:
+ [Count-Min Sketch](https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch)
+ [Count-Sketch](https://www.cs.rutgers.edu/~farach/pubs/FrequentStream.pdf)
+ [AMS Sketch](https://polylogblog.wordpress.com/2009/09/27/bite-sized-stream-ams-sketching/)
+ [Distinct Elements Counter](http://www.cs.dartmouth.edu/~ac/Teach/CS49-Fall11/Notes/lecnotes.pdf)(section 3)
+ [Reservoir sampling](https://en.wikipedia.org/wiki/Reservoir_sampling)
+ [Streaming Submodular Maximization](http://las.ethz.ch/files/badanidiyuru14streaming.pdf)
+ [Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter) and its variants
+ ...




## Dependencies
+ CMake (>= 2.8.7)
+ C++11 support required
+ [Armadillo](http://arma.sourceforge.net/) (optional, required by some features)

## How to Compile
The source code compiles to static library.

Step 1, clone to local machine

    $ git clone https://github.com/jiecchen/StreamingCC.git

Step 2, compile the library

    $ cd StreamingCC/
    $ cmake .
    $ make

Step 3, install the library to system

    $ sudo make install

## Example
Suppose you have `sampling.cc` with the following code,
``` c++
#include <streamingcc>
#include <iostream>
using namespace streamingcc;

int main() {
    // create an object which will maintain
    // 10 samples (with replacement) dynamically
    ReservoirSampler<int> rsmp(10);
    // sample from a data stream with length 1,000,000
    for (int i = 0; i < 1000000; i++)
        rsmp.ProcessItem(i);

    // print the samples
    for (auto sample: rsmp.GetSamples())
        std::cout << sample << " ";
    std::cout << std::endl;
    return 0;
}
```

Now compile the code:

    $ g++ -std=c++11 -O3 -o sampling sampling.cc -lstreamingcc

It will generate an executable file `sampling`. Run the binary with

    $ ./sampling
    916749 93283 843814 534073 877348 445467 369729 163394 67058 212209 

## Recently Included
- library to sample from noisy data, see [readme](https://github.com/jiecchen/StreamingCC/blob/master/src/robust_l0_sampling/README.md) for more information.
- library for construct coreset for a dataset so that one can do clustering with outliers, see [readme](https://github.com/jiecchen/StreamingCC/blob/master/src/dist_clustering/README.md) for more information.
- library for embedding that preserve edit distance, see [readme](https://github.com/jiecchen/StreamingCC/blob/master/src/embed_join/readme.txt) for details.

## TODO
- Add more docs
- Add more python wrappers
- Add more examples
- Unify the interface for recently added library

## Credit

### Maintainer
- Jiecao Chen <chenjiecao@gmail.com> (currently supported by NSF CCF-1525024)

### Other contributor(s)
- Qin Zhang <qzhangcs@indiana.edu>
- Haoyu Zhang <hz30@indiana.edu>
## License
MIT License
