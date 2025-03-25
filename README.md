# learned index db

a simple in-memory database that uses learned index structures instead of traditional b-trees or hash tables

## what is this

instead of using normal indexing like redis does, this db uses machine learning (linear regression) to predict where keys are stored. the idea comes from research that shows learned indexes can be faster for certain data distributions.

## how it works

- stores key-value pairs in memory
- uses a linear model to predict positions of keys
- trains the model as you insert data
- does a small search around predicted position to find exact match

## project structure

```
.
├── src/         # source files
├── include/     # header files
├── bin/         # compiled binary
├── build/       # object files
├── Makefile
└── README.md
```

## building

```bash
make
```

## running

```bash
make run
# or just
./bin/server
```

## commands

- `SET key value` - store a value
- `GET key` - retreive a value
- `DEL key` - delete a key
- `EXISTS key` - check if key exists
- `KEYS` - list all keys
- `SIZE` - number of keys
- `STATS` - show database stats
- `CLEAR` - clear screen
- `HELP` - show help
- `QUIT` - exit

## example

```
learndb> SET name alice
✓ OK
learndb> SET age 25
✓ OK
learndb> GET name
→ "alice"
learndb> KEYS

  1) age
  2) name

learndb> STATS

Database Statistics:
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  Keys stored:  2
  Index type:   Learned (linear regression)
  Storage:      In-memory
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
```

## benchmarking

run benchmarks against redis to see performance comparison:

```bash
# install dependencies first
pip3 install redis matplotlib jupyter

# run benchmark (default 1000 ops)
python3 benchmark.py 5000

# open jupyter notebook for visualizations
jupyter notebook benchmark_analysis.ipynb
```

### benchmark results (5000 ops)

**throughput:**
- LearnDB SET: 18,737 ops/sec
- LearnDB GET: 194,366 ops/sec (25x faster than redis!)
- LearnDB MIXED: 28,144 ops/sec

**speedup over redis:**
- SET operations: 2.4x faster
- GET operations: 25.2x faster
- MIXED operations: 3.6x faster

the learned index really shines on read-heavy workloads!

## research paper

this implementation is based on:

**"The Case for Learned Index Structures"**
*Tim Kraska, Alex Beutel, Ed H. Chi, Jeffrey Dean, Neoklis Polyzotis*
SIGMOD 2018

- **paper:** https://arxiv.org/abs/1712.01208
- **citation:** see [CITATION.md](CITATION.md) for full citation details

the paper shows that learned indexes can outperform traditional b-trees by up to 70% in speed while using 10x less memory. our simple linear regression implementation demonstrates these core concepts.

## notes

this is a basic implementation for understanding how learned indexes work. its not production ready but shows the core concept of using ml models instead of traditional data structures for indexing.
