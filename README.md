# Pretty quick code for the SIR model on temporal networks
It follows the model descriptions in this blog post: https://petterhol.me/2018/07/20/getting-down-to-the-brass-tacks-of-sir-on-temporal-networks/ The algorithm is discussed here: https://petterhol.me/2018/07/23/crazy-fast-code-for-sir-on-temporal-networks/

To get it running you need:

1. The Mersenne Twister from here http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/ in a parallel directory to the one of the code
2. Make dictionaries o (for complier object files) and .state (for RNG state files)
3. Compile it by make (it has some generic optimization flags now). If you want to omit the timing, compile it without the `-DTIME` flag.
4. Run it like

```./tsir nwk/office.csv .state/0 0.2 5```

The first argument is the file containing the network. The input format is a blank-space separated contact list (vertex-id 1, vertex-id 2, time) assuming the vertex-id numbers are between 0 and _N_ - 1. The program sets _N_ as one plus the largest index observed, so if your network has isolates make sure at least the largest id has degree > 0. It also assumes the contact list is sorted in time and starts with time 0. To check if your file format is correct, there is a Python program nwkchk.py. You can run it as `python nwkchk.py filename` If something is stange with your file, it will let you know. The example network in the nwk folder comes from: http://www.sociopatterns.org/datasets/contacts-in-a-workplace/ 

The second argument is a RNG state file. If the file does not exist it is generated. When the program exits it overwrites the file with the current state. This arrangement saves a little time of RNG thermalization (or, equivalently, increases the statistic quality vs seeding the RNG by a number). If you want to run the code from the same sequence of random numbers either comment out the line where the state is written to the disk, or copy a state file before every run. If you have many batch jobs in parallel, make sure they each are running with different state files, otherwise reuse the state files as much as possible.

The third argument is the per-contact infection rate. The fourth is the recovery rate in time units of the duration of the data. If you have the recovery rate in days, you need to divide it by the duration of the data to get the value that goes here.

For more comments, see the files. It should a be relatively easy read for a C coder.
