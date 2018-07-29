# Crazy fast code for the SIR model on temporal networks
It follows the model descriptions in this blog post: https://petterhol.me/2018/07/20/getting-down-to-the-brass-tacks-of-sir-on-temporal-networks/ The algorithm is discussed here: https://petterhol.me/2018/07/23/crazy-fast-code-for-sir-on-temporal-networks/

To get it running you need:

1. Make a directory o (for complier object files)
2. Compile it by make and GCC* (it has some generic optimization flags now). If you want to omit the timing, compile it without the `-DTIME` flag.
3. Run it like

```./tsir nwk/office.csv 0.2 5```

The first argument is the file containing the network. The input format is a blank-space separated contact list (vertex-id 1, vertex-id 2, time) assuming the vertex-id numbers are between 0 and _N_ - 1. The program sets _N_ as one plus the largest index observed, so if your network has isolates make sure at least the largest id has degree > 0. It also assumes the contact list is sorted in time and starts with time 0. To check if your file format is correct, there is a Python program nwkchk.py. You can run it as `python nwkchk.py filename` If something is stange with your file, it will let you know. The example network in the nwk folder comes from: http://www.sociopatterns.org/datasets/contacts-in-a-workplace/

The third argument is the per-contact infection rate. The fourth is the recovery rate in time units of the duration of the data. If you have the recovery rate in days, you need to divide it by the duration of the data to get the value that goes here.

For more comments, see the files. It should a be relatively easy read for a C coder.

\* Clang does not handle the bit-rotation of the RNG efficiently (according to the RNG, for Clang one might want to use another RNG).
