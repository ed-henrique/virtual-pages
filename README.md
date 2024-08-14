# Virtual Pages

Simple example of how virtual memory pages work.

We simply take [this file](./big-file.txt) (which is just the bible as text, and can be found [here](https://github.com/mxw/grmr/blob/master/src/finaltests/bible.txt)) and load it into memory, checking how many virtual memory pages we needed to allocate it.

## How to Compile and Run

### From Docker

```bash
docker run eduhenfm/virtual-pages
```

### From Source

```bash
# Clone
git clone git@github.com:ed-henrique/virtual-pages.git
cd virtual-pages

# Compile
gcc -O3 -Wall -Wextra -o main main.c 

# Run
./main
```

## FAQ

1. How to check current page size?

```bash
getconf PAGESIZE # It will be 4096 most of the time
```

## Reference

Most of this info came from this excellent [course](https://courses.grainger.illinois.edu/cs240/sp2021/).
