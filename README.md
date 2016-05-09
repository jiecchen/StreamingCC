StreamingCC
----------------------
A C++ template library for summarizing data streams.

[![TravisCI](https://travis-ci.org/jiecchen/StreamingCC.svg?branch=master)](https://travis-ci.org/jiecchen/StreamingCC)
[![DUB](https://img.shields.io/dub/l/vibe-d.svg)]()

## Algorithms
`StreamingCC` implements various [streaming algorithms](https://en.wikipedia.org/wiki/Streaming_algorithm) and [probabilistic data structures](https://en.wikipedia.org/wiki/Category:Probabilistic_data_structures). They can be used to effectively summarize the data stream even when data is too large to fit into memory.

Algorithms/Data Structures included in `StreamingCC`:
+ [Count-Min Sketch](https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch)
+ [Count-Sketch](https://www.cs.rutgers.edu/~farach/pubs/FrequentStream.pdf)
+ [AMS Sketch](https://polylogblog.wordpress.com/2009/09/27/bite-sized-stream-ams-sketching/)
+ [Distinct Elements Counter](http://www.cs.dartmouth.edu/~ac/Teach/CS49-Fall11/Notes/lecnotes.pdf)(section 3)
+ [Reservoir sampling](https://en.wikipedia.org/wiki/Reservoir_sampling)
+ [Streaming Submodular Maximization](http://las.ethz.ch/files/badanidiyuru14streaming.pdf)
+ [Bloom Filter](https://en.wikipedia.org/wiki/Bloom_filter) and its variants
+ ...






## Documentation
+ See [StreamingCC](http://xmerge.me/StreamingCC/) to get started.
+ See [API Docs](http://xmerge.me/StreamingCC-api) to dive straight into API.

## Dependencies
+ CMake (>= 2.8.7)
+ C++11 support required
+ [Armadillo](http://arma.sourceforge.net/) (optional, required by some features)

## How to Compile
The source code compiles to static library
See [xmerge.me/StreamingCC](http://xmerge.me/StreamingCC/) for details.

## Credit

### Maintainer
- Jiecao Chen <chenjiecao@gmail.com> (currently supported by NSF CCF-1525024)

### Other contributor(s)
- Qin Zhang <qzhangcs@indiana.edu>

## License
MIT License
