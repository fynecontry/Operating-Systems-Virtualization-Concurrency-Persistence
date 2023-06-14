# Summary

We have introduced the basic ideas behind scheduling and developed two families of approaches:

- The first runs the shortest job remaining and thus optimizes turnaround time; 
- The second alternates between all jobs and thus optimizes response time. 

Both are bad where the other is good, alas, an inherent trade-off common in systems. 
We have also seen how we might incorporate I/O into the picture, but have still not solved the problem​ of the fundamental inability of the OS to see into the future. 
Shortly, we will see how to overcome this problem, by building a scheduler that uses the recent past to predict the future. 
This scheduler is known as the **multi-level feedback queue**.

## Simulator
Using the provided [scheduler.py](https://github.com/fynecontry/Operating-Systems-Virtualization-Concurrency-Persistence/blob/main/virtualization/cpu_scheduling/scheduler.py) answer the following questions.

#### 1. Compute the response time and turnaround time when running three jobs of length 200 with the `SJF` and `FIFO` schedulers.
```
root@educative:/usr/local/educative/code/HW-Scheduler# ./scheduler.py -p SJF -j 3 -m 200 -c
ARG policy SJF
ARG jobs 3
ARG maxlen 200
ARG seed 0

Here is the job list, with the run time of each job: 
  Job 0 ( length = 169 )
  Job 1 ( length = 152 )
  Job 2 ( length = 85 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 2 for 85.00 secs ( DONE at 85.00 )
  [ time  85 ] Run job 1 for 152.00 secs ( DONE at 237.00 )
  [ time 237 ] Run job 0 for 169.00 secs ( DONE at 406.00 )

Final statistics:
  Job   2 -- Response: 0.00  Turnaround 85.00  Wait 0.00
  Job   1 -- Response: 85.00  Turnaround 237.00  Wait 85.00
  Job   0 -- Response: 237.00  Turnaround 406.00  Wait 237.00

  Average -- Response: 107.33  Turnaround 242.67  Wait 107.33

root@educative:/usr/local/educative/code/HW-Scheduler# ./scheduler.py -p FIFO -j 3 -m 200 -c
ARG policy FIFO
ARG jobs 3
ARG maxlen 200
ARG seed 0

Here is the job list, with the run time of each job: 
  Job 0 ( length = 169 )
  Job 1 ( length = 152 )
  Job 2 ( length = 85 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 169.00 secs ( DONE at 169.00 )
  [ time 169 ] Run job 1 for 152.00 secs ( DONE at 321.00 )
  [ time 321 ] Run job 2 for 85.00 secs ( DONE at 406.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 169.00  Wait 0.00
  Job   1 -- Response: 169.00  Turnaround 321.00  Wait 169.00
  Job   2 -- Response: 321.00  Turnaround 406.00  Wait 321.00

  Average -- Response: 163.33  Turnaround 298.67  Wait 163.33
```


#### 2. Now do the same but with jobs of different lengths: 100, 200, and 300.

```
root@educative:/usr/local/educative/code/HW-Scheduler# ./scheduler.py -p SJF -l 100,200,300 -c
ARG policy SJF
ARG jlist 100,200,300

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00

  Average -- Response: 133.33  Turnaround 333.33  Wait 133.33
```

#### 3. Now do the same, but also with the RR scheduler and a time-slice of 1.
```
root@educative:/usr/local/educative/code/HW-Scheduler# ./scheduler.py -p RR -j 3 -m 200 -q 1 -c
ARG policy RR
ARG jobs 3
ARG maxlen 200
ARG seed 0

Here is the job list, with the run time of each job: 
  Job 0 ( length = 169 )
  Job 1 ( length = 152 )
  Job 2 ( length = 85 )


** Solutions **

Execution trace:
  [ time   0 ] Run job   0 for 1.00 secs
  [ time   1 ] Run job   1 for 1.00 secs
  [ time   2 ] Run job   2 for 1.00 secs
  [ time   3 ] Run job   0 for 1.00 secs
  [ time   4 ] Run job   1 for 1.00 secs
  [ time   5 ] Run job   2 for 1.00 secs
  [ time   6 ] Run job   0 for 1.00 secs
  [ time   7 ] Run job   1 for 1.00 secs
  [ time   8 ] Run job   2 for 1.00 secs
  [ time   9 ] Run job   0 for 1.00 secs
  [ time  10 ] Run job   1 for 1.00 secs
  [ time  11 ] Run job   2 for 1.00 secs
  [ time  12 ] Run job   0 for 1.00 secs
  [ time  13 ] Run job   1 for 1.00 secs
  [ time  14 ] Run job   2 for 1.00 secs
  [ time  15 ] Run job   0 for 1.00 secs
  [ time  16 ] Run job   1 for 1.00 secs
  [ time  17 ] Run job   2 for 1.00 secs
  [ time  18 ] Run job   0 for 1.00 secs
  [ time  19 ] Run job   1 for 1.00 secs
  [ time  20 ] Run job   2 for 1.00 secs
  [ time  21 ] Run job   0 for 1.00 secs
  [ time  22 ] Run job   1 for 1.00 secs
  [ time  23 ] Run job   2 for 1.00 secs
  [ time  24 ] Run job   0 for 1.00 secs
  [ time  25 ] Run job   1 for 1.00 secs
  [ time  26 ] Run job   2 for 1.00 secs
  [ time  27 ] Run job   0 for 1.00 secs
  [ time  28 ] Run job   1 for 1.00 secs
  [ time  29 ] Run job   2 for 1.00 secs
  [ time  30 ] Run job   0 for 1.00 secs
  [ time  31 ] Run job   1 for 1.00 secs
  [ time  32 ] Run job   2 for 1.00 secs
  [ time  33 ] Run job   0 for 1.00 secs
  [ time  34 ] Run job   1 for 1.00 secs
  [ time  35 ] Run job   2 for 1.00 secs
  [ time  36 ] Run job   0 for 1.00 secs
  [ time  37 ] Run job   1 for 1.00 secs
  [ time  38 ] Run job   2 for 1.00 secs
  [ time  39 ] Run job   0 for 1.00 secs
  [ time  40 ] Run job   1 for 1.00 secs
  [ time  41 ] Run job   2 for 1.00 secs
  [ time  42 ] Run job   0 for 1.00 secs
  [ time  43 ] Run job   1 for 1.00 secs
  [ time  44 ] Run job   2 for 1.00 secs
  [ time  45 ] Run job   0 for 1.00 secs
  [ time  46 ] Run job   1 for 1.00 secs
  [ time  47 ] Run job   2 for 1.00 secs
  [ time  48 ] Run job   0 for 1.00 secs
  [ time  49 ] Run job   1 for 1.00 secs
  [ time  50 ] Run job   2 for 1.00 secs
  [ time  51 ] Run job   0 for 1.00 secs
  [ time  52 ] Run job   1 for 1.00 secs
  [ time  53 ] Run job   2 for 1.00 secs
  [ time  54 ] Run job   0 for 1.00 secs
  [ time  55 ] Run job   1 for 1.00 secs
  [ time  56 ] Run job   2 for 1.00 secs
  [ time  57 ] Run job   0 for 1.00 secs
  [ time  58 ] Run job   1 for 1.00 secs
  [ time  59 ] Run job   2 for 1.00 secs
  [ time  60 ] Run job   0 for 1.00 secs
  [ time  61 ] Run job   1 for 1.00 secs
  [ time  62 ] Run job   2 for 1.00 secs
  [ time  63 ] Run job   0 for 1.00 secs
  [ time  64 ] Run job   1 for 1.00 secs
  [ time  65 ] Run job   2 for 1.00 secs
  [ time  66 ] Run job   0 for 1.00 secs
  [ time  67 ] Run job   1 for 1.00 secs
  [ time  68 ] Run job   2 for 1.00 secs
  [ time  69 ] Run job   0 for 1.00 secs
  [ time  70 ] Run job   1 for 1.00 secs
  [ time  71 ] Run job   2 for 1.00 secs
  [ time  72 ] Run job   0 for 1.00 secs
  [ time  73 ] Run job   1 for 1.00 secs
  [ time  74 ] Run job   2 for 1.00 secs
  [ time  75 ] Run job   0 for 1.00 secs
  [ time  76 ] Run job   1 for 1.00 secs
  [ time  77 ] Run job   2 for 1.00 secs
  [ time  78 ] Run job   0 for 1.00 secs
  [ time  79 ] Run job   1 for 1.00 secs
  [ time  80 ] Run job   2 for 1.00 secs
  [ time  81 ] Run job   0 for 1.00 secs
  [ time  82 ] Run job   1 for 1.00 secs
  [ time  83 ] Run job   2 for 1.00 secs
  [ time  84 ] Run job   0 for 1.00 secs
  [ time  85 ] Run job   1 for 1.00 secs
  [ time  86 ] Run job   2 for 1.00 secs
  [ time  87 ] Run job   0 for 1.00 secs
  [ time  88 ] Run job   1 for 1.00 secs
  [ time  89 ] Run job   2 for 1.00 secs
  [ time  90 ] Run job   0 for 1.00 secs
  [ time  91 ] Run job   1 for 1.00 secs
  [ time  92 ] Run job   2 for 1.00 secs
  [ time  93 ] Run job   0 for 1.00 secs
  [ time  94 ] Run job   1 for 1.00 secs
  [ time  95 ] Run job   2 for 1.00 secs
  [ time  96 ] Run job   0 for 1.00 secs
  [ time  97 ] Run job   1 for 1.00 secs
  [ time  98 ] Run job   2 for 1.00 secs
  [ time  99 ] Run job   0 for 1.00 secs
  [ time 100 ] Run job   1 for 1.00 secs
  [ time 101 ] Run job   2 for 1.00 secs
  [ time 102 ] Run job   0 for 1.00 secs
  [ time 103 ] Run job   1 for 1.00 secs
  [ time 104 ] Run job   2 for 1.00 secs
  [ time 105 ] Run job   0 for 1.00 secs
  [ time 106 ] Run job   1 for 1.00 secs
  [ time 107 ] Run job   2 for 1.00 secs
  [ time 108 ] Run job   0 for 1.00 secs
  [ time 109 ] Run job   1 for 1.00 secs
  [ time 110 ] Run job   2 for 1.00 secs
  [ time 111 ] Run job   0 for 1.00 secs
  [ time 112 ] Run job   1 for 1.00 secs
  [ time 113 ] Run job   2 for 1.00 secs
  [ time 114 ] Run job   0 for 1.00 secs
  [ time 115 ] Run job   1 for 1.00 secs
  [ time 116 ] Run job   2 for 1.00 secs
  [ time 117 ] Run job   0 for 1.00 secs
  [ time 118 ] Run job   1 for 1.00 secs
  [ time 119 ] Run job   2 for 1.00 secs
  [ time 120 ] Run job   0 for 1.00 secs
  [ time 121 ] Run job   1 for 1.00 secs
  [ time 122 ] Run job   2 for 1.00 secs
  [ time 123 ] Run job   0 for 1.00 secs
  [ time 124 ] Run job   1 for 1.00 secs
  [ time 125 ] Run job   2 for 1.00 secs
  [ time 126 ] Run job   0 for 1.00 secs
  [ time 127 ] Run job   1 for 1.00 secs
  [ time 128 ] Run job   2 for 1.00 secs
  [ time 129 ] Run job   0 for 1.00 secs
  [ time 130 ] Run job   1 for 1.00 secs
  [ time 131 ] Run job   2 for 1.00 secs
  [ time 132 ] Run job   0 for 1.00 secs
  [ time 133 ] Run job   1 for 1.00 secs
  [ time 134 ] Run job   2 for 1.00 secs
  [ time 135 ] Run job   0 for 1.00 secs
  [ time 136 ] Run job   1 for 1.00 secs
  [ time 137 ] Run job   2 for 1.00 secs
  [ time 138 ] Run job   0 for 1.00 secs
  [ time 139 ] Run job   1 for 1.00 secs
  [ time 140 ] Run job   2 for 1.00 secs
  [ time 141 ] Run job   0 for 1.00 secs
  [ time 142 ] Run job   1 for 1.00 secs
  [ time 143 ] Run job   2 for 1.00 secs
  [ time 144 ] Run job   0 for 1.00 secs
  [ time 145 ] Run job   1 for 1.00 secs
  [ time 146 ] Run job   2 for 1.00 secs
  [ time 147 ] Run job   0 for 1.00 secs
  [ time 148 ] Run job   1 for 1.00 secs
  [ time 149 ] Run job   2 for 1.00 secs
  [ time 150 ] Run job   0 for 1.00 secs
  [ time 151 ] Run job   1 for 1.00 secs
  [ time 152 ] Run job   2 for 1.00 secs
  [ time 153 ] Run job   0 for 1.00 secs
  [ time 154 ] Run job   1 for 1.00 secs
  [ time 155 ] Run job   2 for 1.00 secs
  [ time 156 ] Run job   0 for 1.00 secs
  [ time 157 ] Run job   1 for 1.00 secs
  [ time 158 ] Run job   2 for 1.00 secs
  [ time 159 ] Run job   0 for 1.00 secs
  [ time 160 ] Run job   1 for 1.00 secs
  [ time 161 ] Run job   2 for 1.00 secs
  [ time 162 ] Run job   0 for 1.00 secs
  [ time 163 ] Run job   1 for 1.00 secs
  [ time 164 ] Run job   2 for 1.00 secs
  [ time 165 ] Run job   0 for 1.00 secs
  [ time 166 ] Run job   1 for 1.00 secs
  [ time 167 ] Run job   2 for 1.00 secs
  [ time 168 ] Run job   0 for 1.00 secs
  [ time 169 ] Run job   1 for 1.00 secs
  [ time 170 ] Run job   2 for 1.00 secs
  [ time 171 ] Run job   0 for 1.00 secs
  [ time 172 ] Run job   1 for 1.00 secs
  [ time 173 ] Run job   2 for 1.00 secs
  [ time 174 ] Run job   0 for 1.00 secs
  [ time 175 ] Run job   1 for 1.00 secs
  [ time 176 ] Run job   2 for 1.00 secs
  [ time 177 ] Run job   0 for 1.00 secs
  [ time 178 ] Run job   1 for 1.00 secs
  [ time 179 ] Run job   2 for 1.00 secs
  [ time 180 ] Run job   0 for 1.00 secs
  [ time 181 ] Run job   1 for 1.00 secs
  [ time 182 ] Run job   2 for 1.00 secs
  [ time 183 ] Run job   0 for 1.00 secs
  [ time 184 ] Run job   1 for 1.00 secs
  [ time 185 ] Run job   2 for 1.00 secs
  [ time 186 ] Run job   0 for 1.00 secs
  [ time 187 ] Run job   1 for 1.00 secs
  [ time 188 ] Run job   2 for 1.00 secs
  [ time 189 ] Run job   0 for 1.00 secs
  [ time 190 ] Run job   1 for 1.00 secs
  [ time 191 ] Run job   2 for 1.00 secs
  [ time 192 ] Run job   0 for 1.00 secs
  [ time 193 ] Run job   1 for 1.00 secs
  [ time 194 ] Run job   2 for 1.00 secs
  [ time 195 ] Run job   0 for 1.00 secs
  [ time 196 ] Run job   1 for 1.00 secs
  [ time 197 ] Run job   2 for 1.00 secs
  [ time 198 ] Run job   0 for 1.00 secs
  [ time 199 ] Run job   1 for 1.00 secs
  [ time 200 ] Run job   2 for 1.00 secs
  [ time 201 ] Run job   0 for 1.00 secs
  [ time 202 ] Run job   1 for 1.00 secs
  [ time 203 ] Run job   2 for 1.00 secs
  [ time 204 ] Run job   0 for 1.00 secs
  [ time 205 ] Run job   1 for 1.00 secs
  [ time 206 ] Run job   2 for 1.00 secs
  [ time 207 ] Run job   0 for 1.00 secs
  [ time 208 ] Run job   1 for 1.00 secs
  [ time 209 ] Run job   2 for 1.00 secs
  [ time 210 ] Run job   0 for 1.00 secs
  [ time 211 ] Run job   1 for 1.00 secs
  [ time 212 ] Run job   2 for 1.00 secs
  [ time 213 ] Run job   0 for 1.00 secs
  [ time 214 ] Run job   1 for 1.00 secs
  [ time 215 ] Run job   2 for 1.00 secs
  [ time 216 ] Run job   0 for 1.00 secs
  [ time 217 ] Run job   1 for 1.00 secs
  [ time 218 ] Run job   2 for 1.00 secs
  [ time 219 ] Run job   0 for 1.00 secs
  [ time 220 ] Run job   1 for 1.00 secs
  [ time 221 ] Run job   2 for 1.00 secs
  [ time 222 ] Run job   0 for 1.00 secs
  [ time 223 ] Run job   1 for 1.00 secs
  [ time 224 ] Run job   2 for 1.00 secs
  [ time 225 ] Run job   0 for 1.00 secs
  [ time 226 ] Run job   1 for 1.00 secs
  [ time 227 ] Run job   2 for 1.00 secs
  [ time 228 ] Run job   0 for 1.00 secs
  [ time 229 ] Run job   1 for 1.00 secs
  [ time 230 ] Run job   2 for 1.00 secs
  [ time 231 ] Run job   0 for 1.00 secs
  [ time 232 ] Run job   1 for 1.00 secs
  [ time 233 ] Run job   2 for 1.00 secs
  [ time 234 ] Run job   0 for 1.00 secs
  [ time 235 ] Run job   1 for 1.00 secs
  [ time 236 ] Run job   2 for 1.00 secs
  [ time 237 ] Run job   0 for 1.00 secs
  [ time 238 ] Run job   1 for 1.00 secs
  [ time 239 ] Run job   2 for 1.00 secs
  [ time 240 ] Run job   0 for 1.00 secs
  [ time 241 ] Run job   1 for 1.00 secs
  [ time 242 ] Run job   2 for 1.00 secs
  [ time 243 ] Run job   0 for 1.00 secs
  [ time 244 ] Run job   1 for 1.00 secs
  [ time 245 ] Run job   2 for 1.00 secs
  [ time 246 ] Run job   0 for 1.00 secs
  [ time 247 ] Run job   1 for 1.00 secs
  [ time 248 ] Run job   2 for 1.00 secs
  [ time 249 ] Run job   0 for 1.00 secs
  [ time 250 ] Run job   1 for 1.00 secs
  [ time 251 ] Run job   2 for 1.00 secs
  [ time 252 ] Run job   0 for 1.00 secs
  [ time 253 ] Run job   1 for 1.00 secs
  [ time 254 ] Run job   2 for 1.00 secs ( DONE at 255.00 )
  [ time 255 ] Run job   0 for 1.00 secs
  [ time 256 ] Run job   1 for 1.00 secs
  [ time 257 ] Run job   0 for 1.00 secs
  [ time 258 ] Run job   1 for 1.00 secs
  [ time 259 ] Run job   0 for 1.00 secs
  [ time 260 ] Run job   1 for 1.00 secs
  [ time 261 ] Run job   0 for 1.00 secs
  [ time 262 ] Run job   1 for 1.00 secs
  [ time 263 ] Run job   0 for 1.00 secs
  [ time 264 ] Run job   1 for 1.00 secs
  [ time 265 ] Run job   0 for 1.00 secs
  [ time 266 ] Run job   1 for 1.00 secs
  [ time 267 ] Run job   0 for 1.00 secs
  [ time 268 ] Run job   1 for 1.00 secs
  [ time 269 ] Run job   0 for 1.00 secs
  [ time 270 ] Run job   1 for 1.00 secs
  [ time 271 ] Run job   0 for 1.00 secs
  [ time 272 ] Run job   1 for 1.00 secs
  [ time 273 ] Run job   0 for 1.00 secs
  [ time 274 ] Run job   1 for 1.00 secs
  [ time 275 ] Run job   0 for 1.00 secs
  [ time 276 ] Run job   1 for 1.00 secs
  [ time 277 ] Run job   0 for 1.00 secs
  [ time 278 ] Run job   1 for 1.00 secs
  [ time 279 ] Run job   0 for 1.00 secs
  [ time 280 ] Run job   1 for 1.00 secs
  [ time 281 ] Run job   0 for 1.00 secs
  [ time 282 ] Run job   1 for 1.00 secs
  [ time 283 ] Run job   0 for 1.00 secs
  [ time 284 ] Run job   1 for 1.00 secs
  [ time 285 ] Run job   0 for 1.00 secs
  [ time 286 ] Run job   1 for 1.00 secs
  [ time 287 ] Run job   0 for 1.00 secs
  [ time 288 ] Run job   1 for 1.00 secs
  [ time 289 ] Run job   0 for 1.00 secs
  [ time 290 ] Run job   1 for 1.00 secs
  [ time 291 ] Run job   0 for 1.00 secs
  [ time 292 ] Run job   1 for 1.00 secs
  [ time 293 ] Run job   0 for 1.00 secs
  [ time 294 ] Run job   1 for 1.00 secs
  [ time 295 ] Run job   0 for 1.00 secs
  [ time 296 ] Run job   1 for 1.00 secs
  [ time 297 ] Run job   0 for 1.00 secs
  [ time 298 ] Run job   1 for 1.00 secs
  [ time 299 ] Run job   0 for 1.00 secs
  [ time 300 ] Run job   1 for 1.00 secs
  [ time 301 ] Run job   0 for 1.00 secs
  [ time 302 ] Run job   1 for 1.00 secs
  [ time 303 ] Run job   0 for 1.00 secs
  [ time 304 ] Run job   1 for 1.00 secs
  [ time 305 ] Run job   0 for 1.00 secs
  [ time 306 ] Run job   1 for 1.00 secs
  [ time 307 ] Run job   0 for 1.00 secs
  [ time 308 ] Run job   1 for 1.00 secs
  [ time 309 ] Run job   0 for 1.00 secs
  [ time 310 ] Run job   1 for 1.00 secs
  [ time 311 ] Run job   0 for 1.00 secs
  [ time 312 ] Run job   1 for 1.00 secs
  [ time 313 ] Run job   0 for 1.00 secs
  [ time 314 ] Run job   1 for 1.00 secs
  [ time 315 ] Run job   0 for 1.00 secs
  [ time 316 ] Run job   1 for 1.00 secs
  [ time 317 ] Run job   0 for 1.00 secs
  [ time 318 ] Run job   1 for 1.00 secs
  [ time 319 ] Run job   0 for 1.00 secs
  [ time 320 ] Run job   1 for 1.00 secs
  [ time 321 ] Run job   0 for 1.00 secs
  [ time 322 ] Run job   1 for 1.00 secs
  [ time 323 ] Run job   0 for 1.00 secs
  [ time 324 ] Run job   1 for 1.00 secs
  [ time 325 ] Run job   0 for 1.00 secs
  [ time 326 ] Run job   1 for 1.00 secs
  [ time 327 ] Run job   0 for 1.00 secs
  [ time 328 ] Run job   1 for 1.00 secs
  [ time 329 ] Run job   0 for 1.00 secs
  [ time 330 ] Run job   1 for 1.00 secs
  [ time 331 ] Run job   0 for 1.00 secs
  [ time 332 ] Run job   1 for 1.00 secs
  [ time 333 ] Run job   0 for 1.00 secs
  [ time 334 ] Run job   1 for 1.00 secs
  [ time 335 ] Run job   0 for 1.00 secs
  [ time 336 ] Run job   1 for 1.00 secs
  [ time 337 ] Run job   0 for 1.00 secs
  [ time 338 ] Run job   1 for 1.00 secs
  [ time 339 ] Run job   0 for 1.00 secs
  [ time 340 ] Run job   1 for 1.00 secs
  [ time 341 ] Run job   0 for 1.00 secs
  [ time 342 ] Run job   1 for 1.00 secs
  [ time 343 ] Run job   0 for 1.00 secs
  [ time 344 ] Run job   1 for 1.00 secs
  [ time 345 ] Run job   0 for 1.00 secs
  [ time 346 ] Run job   1 for 1.00 secs
  [ time 347 ] Run job   0 for 1.00 secs
  [ time 348 ] Run job   1 for 1.00 secs
  [ time 349 ] Run job   0 for 1.00 secs
  [ time 350 ] Run job   1 for 1.00 secs
  [ time 351 ] Run job   0 for 1.00 secs
  [ time 352 ] Run job   1 for 1.00 secs
  [ time 353 ] Run job   0 for 1.00 secs
  [ time 354 ] Run job   1 for 1.00 secs
  [ time 355 ] Run job   0 for 1.00 secs
  [ time 356 ] Run job   1 for 1.00 secs
  [ time 357 ] Run job   0 for 1.00 secs
  [ time 358 ] Run job   1 for 1.00 secs
  [ time 359 ] Run job   0 for 1.00 secs
  [ time 360 ] Run job   1 for 1.00 secs
  [ time 361 ] Run job   0 for 1.00 secs
  [ time 362 ] Run job   1 for 1.00 secs
  [ time 363 ] Run job   0 for 1.00 secs
  [ time 364 ] Run job   1 for 1.00 secs
  [ time 365 ] Run job   0 for 1.00 secs
  [ time 366 ] Run job   1 for 1.00 secs
  [ time 367 ] Run job   0 for 1.00 secs
  [ time 368 ] Run job   1 for 1.00 secs
  [ time 369 ] Run job   0 for 1.00 secs
  [ time 370 ] Run job   1 for 1.00 secs
  [ time 371 ] Run job   0 for 1.00 secs
  [ time 372 ] Run job   1 for 1.00 secs
  [ time 373 ] Run job   0 for 1.00 secs
  [ time 374 ] Run job   1 for 1.00 secs
  [ time 375 ] Run job   0 for 1.00 secs
  [ time 376 ] Run job   1 for 1.00 secs
  [ time 377 ] Run job   0 for 1.00 secs
  [ time 378 ] Run job   1 for 1.00 secs
  [ time 379 ] Run job   0 for 1.00 secs
  [ time 380 ] Run job   1 for 1.00 secs
  [ time 381 ] Run job   0 for 1.00 secs
  [ time 382 ] Run job   1 for 1.00 secs
  [ time 383 ] Run job   0 for 1.00 secs
  [ time 384 ] Run job   1 for 1.00 secs
  [ time 385 ] Run job   0 for 1.00 secs
  [ time 386 ] Run job   1 for 1.00 secs
  [ time 387 ] Run job   0 for 1.00 secs
  [ time 388 ] Run job   1 for 1.00 secs ( DONE at 389.00 )
  [ time 389 ] Run job   0 for 1.00 secs
  [ time 390 ] Run job   0 for 1.00 secs
  [ time 391 ] Run job   0 for 1.00 secs
  [ time 392 ] Run job   0 for 1.00 secs
  [ time 393 ] Run job   0 for 1.00 secs
  [ time 394 ] Run job   0 for 1.00 secs
  [ time 395 ] Run job   0 for 1.00 secs
  [ time 396 ] Run job   0 for 1.00 secs
  [ time 397 ] Run job   0 for 1.00 secs
  [ time 398 ] Run job   0 for 1.00 secs
  [ time 399 ] Run job   0 for 1.00 secs
  [ time 400 ] Run job   0 for 1.00 secs
  [ time 401 ] Run job   0 for 1.00 secs
  [ time 402 ] Run job   0 for 1.00 secs
  [ time 403 ] Run job   0 for 1.00 secs
  [ time 404 ] Run job   0 for 1.00 secs
  [ time 405 ] Run job   0 for 1.00 secs ( DONE at 406.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 406.00  Wait 237.00
  Job   1 -- Response: 1.00  Turnaround 389.00  Wait 237.00
  Job   2 -- Response: 2.00  Turnaround 255.00  Wait 170.00

  Average -- Response: 1.00  Turnaround 350.00  Wait 214.67
```

#### 4. For what types of workloads does SJF deliver the same turnaround times as FIFO?
> Workloads which arrive in ascending order will have the same turnaround times using SJF or FIFO, as well as workloads of equal lengths

#### 5. For what types of workloads and quantum lengths does SJF deliver the same response times as RR?
> This is achieveable if quantum length is **always** equal to the shortest job in the queue and the length of all workloads are known in advance so the shortest job runs first.

#### 6. What happens to response time with SJF as job lengths increase? Can you use the simulator to demonstrate the trend?
```
root@educative:/usr/local/educative/code/HW-Scheduler# ./scheduler.py -p SJF -l 100,200,300,400 -c
ARG policy SJF
ARG jlist 100,200,300,400

Here is the job list, with the run time of each job: 
  Job 0 ( length = 100.0 )
  Job 1 ( length = 200.0 )
  Job 2 ( length = 300.0 )
  Job 3 ( length = 400.0 )


** Solutions **

Execution trace:
  [ time   0 ] Run job 0 for 100.00 secs ( DONE at 100.00 )
  [ time 100 ] Run job 1 for 200.00 secs ( DONE at 300.00 )
  [ time 300 ] Run job 2 for 300.00 secs ( DONE at 600.00 )
  [ time 600 ] Run job 3 for 400.00 secs ( DONE at 1000.00 )

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00
  Job   3 -- Response: 600.00  Turnaround 1000.00  Wait 600.00

  Average -- Response: 250.00  Turnaround 500.00  Wait 250.00
```

#### 7. What happens to response time with RR as quantum lengths increase? Can you write an equation that gives the worst-case response time, given N jobs?
> Response time increases as quantum lengths increase in RR algorithm. `worst-case response time = (N-1) * quantum_length`