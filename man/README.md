# How to generate documentation

Documentation is generated using `ronn` tool from https://github.com/rtomayko/ronn.

## Install dependent packages

```shell
dnf -y install rubygem-ronn
```

# Generate man pages

Modify man page file ending with `.ronn` which should Markdown formatted. 
Regenerate documentation:

```shell
ronn --roff dosgen.1.ronn
```

You can preview the output file by running:
```shell
man ./dosgen.1
```

Or use quick preview:
```shell
ronn --man dosgen.1.ronn
```

