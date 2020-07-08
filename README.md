# Crazy fast code for the SIR model on temporal networks
It follows the model descriptions in this blog post: [https://petterhol.me/2018/07/20/getting-down-to-the-brass-tacks-of-sir-on-temporal-networks/](Getting down to the brass tacks) The algorithm is discussed here: [https://petterhol.me/2018/07/23/crazy-fast-code-for-sir-on-temporal-networks/](Crazy fast code for the SIR model on temporal networks)

To get it running you need:

1. Make a directory o (for complier object files)
2. Compile it by gcc by typing `make`.
3. Run it via a Python wrapper.

I supply two Python wrappers: One for simply calculating the average outbreak size given a network and the two parameters (per-contact transmission probability and recovery rate). You can run it like:

```python3 tsir_single.py gallery01.tsv 0.5 2```

Another python wrapper is for scanning the parameter space (at least a part of it) and plotting a heat map of the outbreak size. You can run that one like:

```python3 tsir_scan.py gallery01.tsv gallery01.pdf```

The first argument is the file containing the network. The input format is a blank-space separated contact list (vertex-id 1, vertex-id 2, time). It will neglect multiple contacts and self-contacts. The example network in the nwk folder comes from: http://www.sociopatterns.org/datasets/

## Notes for the July 2020 update

I have tried to simplify the code without making it slower by moving preprocessing to the Python wrapper. (Running the code from Python was anyway more or less needed to conveniently seed the C code with a 64-bit uint.) It uses 10^6 averages, which is probably more than enough for larger data sets.
