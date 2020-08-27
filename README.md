# Crazy fast code for the SIR model on temporal networks
This code is described in this preprint https://arxiv.org/abs/2007.14386 So if you need to cite it:
>@unpublished{holme_tsir,
>  author={Petter Holme},
>  title={Fast and principled simulations of the {SIR} model on temporal networks},
>  note={e-print arXiv:2007.14386},
>  year=2020}
>  }


To get it running you need:

1. Make a directory o (for complier object files)
2. Compile it by gcc by typing `make`.
3. Run it via a Python wrapper.

I supply two Python wrappers: One for simply calculating the average outbreak size given a network and the two parameters (per-contact transmission probability and recovery rate). You can run it like:

```python3 tsir_single.py gallery01.tsv 0.5 2```

The first argument is the file containing the network. The input format is a blank-space separated contact list (vertex-id 1, vertex-id 2, time). It will neglect multiple contacts and self-contacts. The example network comes from: http://www.sociopatterns.org/datasets/ The second argument is the transmission probability, the third is the recovery rate.

Another python wrapper is for scanning the parameter space (at least a part of it) and plotting a heat map of the outbreak size. You can run that one like:

```python3 tsir_scan.py gallery01.tsv gallery01.pdf```

The last argument is the name of the output figure file. The suffix determines the image format.

## Notes for the July 2020 update

I have tried to simplify the code without making it slower by moving preprocessing to the Python wrapper. (Running the code from Python was anyway more or less needed to conveniently seed the C code with a random 64-bit uint.) It uses 10^6 averages, which is probably more than enough for larger data sets.

I have refrained from expanding this into a full Python library mostly because the research projects I can imagine building on this code would anyway need to add something to the C program. For example, adding measurements about individual nodes or links should be straightforward.
