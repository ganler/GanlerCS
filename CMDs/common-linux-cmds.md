[TOC]

## Conda

### Create the same required env as base

```bash
conda create -n ENVNAME --file <(conda list -n base --export | grep "^python=\|^pip=") --yes
```

## GCC

### Version Checking In CLI

```bash
ISGCCGEV6=${expr $(gcc -dumpversion | cut -f1 -d.) \>= 6}
[ $ISGCCGEV6 = 1 ] && ... # If Then.
```