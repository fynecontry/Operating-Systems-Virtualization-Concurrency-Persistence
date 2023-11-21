# map-reduce.h

[map-reduce.h](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/tree/main/Projects/map-reduce/map-reduce.h) is library created as a simplified version of Google's large-scale parallel data processing framework [MapReduce](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf).

It is built to work on a single machine using [POSIX threads](https://man7.org/linux/man-pages/man3/pthread_create.3.html) for mapping and reducing jobs and [mutexes](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html) for threads synchronization.

## Architecture

![word_count map-reduce](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/assets/27024731/da0260a6-8be5-41ab-8916-b3e8d21676f9)

The MapReduce infrastructure supports the execution of user-defined `Map()` and `Reduce()` functions which are defined in word-count.c

As from the original paper: "Map(), written by the user, takes an input pair and produces a set of intermediate key/value pairs. The MapReduce library groups together all intermediate values associated with the same intermediate key K and passes them to the Reduce() function."

"The Reduce() function, also written by the user, accepts an intermediate key K and a set of values for that key. It merges together these values to form a possibly smaller set of values; typically just zero or one output value is produced per Reduce() invocation. The intermediate values are supplied to the user's reduce function via an iterator."

## How it works
`MR_RUN` coordinates and creates the mappers which execute the user-defined `Map` Function.

Mappers then call `MR_EMIT` routine which passes individual KV pairs to a `hashtable_t`

`MR_RUN` then creates the Reducers which executes Reduce_Wrapper, this iterates through all partitions in the hashtable and then calls `Reduce` function for merging and then prints to screen the aggregated values.

## Example
```
map-reduce$ ./word_count file1.txt file2.txt


====Starting mapping operations====

Raindrops 1
fall 1
from 1
the 1
dark 1
clouds 1
above 1
Puddles 1
form 1
on 1
the 1
wet 1
ground 1
reflecting 1
the 1
gray 1
sky 1
The 1
rain 1
provides 1
nourishment 1
to 1
the 1
thirsty 1
plants 1
and 1
flowers 1
Rain 1
is 1
essential 1
for 1
the 1
growth 1
of 1
crops 1
and 1
the 1
health 1
of 1
the 1
environment 1
People 1
stay 1
indoors 1
listening 1
to 1
the 1
rhythmic 1
sound 1
of 1
raindrops 1
on 1
their 1
rooftops 1
The 1
sun 1
shines 1
brightly 1
in 1
the 1
sky 1
Birds 1
chirp 1
happily 1
as 1
they 1
fly 1
from 1
tree 1
to 1
tree 1
The 1
tree 1
branches 1
sway 1
gently 1
in 1
the 1
breeze 1
Sunflowers 1
bloom 1
with 1
vibrant 1
yellow 1
petals 1
The 1
sun's 1
rays 1
illuminate 1
the 1
entire 1
landscape 1
creating 1
a 1
warm 1
and 1
cheerful 1
atmosphere 1
====Mappers are done====

====Starting reduce operations====

with 1
warm 1
landscape 1
yellow 1
bloom 1
gently 1
tree 3
they 1
brightly 1
sun 1
sound 1
listening 1
essential 1
Rain 1
to 3
nourishment 1
Puddles 1
dark 1
branches 1
from 2
shines 1
The 4
cheerful 1
entire 1
Sunflowers 1
rhythmic 1
stay 1
environment 1
growth 1
for 1
rain 1
ground 1
wet 1
on 2
Raindrops 1
sun's 1
vibrant 1
sway 1
chirp 1
in 2
raindrops 1
of 3
is 1
flowers 1
sky 2
clouds 1
the 11
atmosphere 1
a 1
creating 1
illuminate 1
rays 1
petals 1
breeze 1
fly 1
as 1
happily 1
Birds 1
rooftops 1
their 1
indoors 1
People 1
health 1
crops 1
and 3
plants 1
thirsty 1
provides 1
gray 1
reflecting 1
form 1
above 1
fall 1
```