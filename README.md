# Arcade Machine

<p align="left">
    <img width="150px" src="https://github.com/thoth-tech/.github/blob/main/images/splashkit.png"/>
</p>

[![GitHub contributors](https://img.shields.io/github/contributors/thoth-tech/arcade-machine?label=Contributors&color=F5A623)](https://github.com/thoth-tech/arcade-machine/graphs/contributors)
[![GitHub issues](https://img.shields.io/github/issues/thoth-tech/arcade-machine?label=Issues&color=F5A623)](https://github.com/thoth-tech/arcade-machine/issues)
[![GitHub pull requests](https://img.shields.io/github/issues-pr/thoth-tech/arcade-machine?label=Pull%20Requests&color=F5A623)](https://github.com/thoth-tech/arcade-machine/pulls)
[![Forks](https://img.shields.io/github/forks/thoth-tech/arcade-machine?label=Forks&color=F5A623)](https://github.com/thoth-tech/arcade-machine/network/members)
[![Stars](https://img.shields.io/github/stars/thoth-tech/arcade-machine?label=Stars&color=F5A623)](https://github.com/thoth-tech/arcade-machine/stargazers)

**Arcade Machine** is an interactive application built to showcase and execute games created with the [SplashKit SDK](https://github.com/thoth-tech/splashkit-core). Designed as a hub for all your SplashKit games, Arcade Machine provides an efficient and user-friendly way to compile, organize, and run games in one place. It’s ideal for both developers and players who want to easily access and test their creations.

## Pre-requisites

### General (All Operating Systems)

- Install the SplashKit SDK by following the [installation guide](https://splashkit.io/installation/).

#### Windows (mingw32)

- Install `make` by running the following command in your terminal:

    ```bash
    pacman -S make
    ```

#### macOS

- Install `make` (if not already installed) by installing Xcode Command Line Tools:

    ```bash
    xcode-select --install
    ```

#### Linux (Debian/Ubuntu)

- Install `make` by running the following command in your terminal:

    ```bash
    sudo apt install make
    ```

## Building Arcade Machine

You can build Arcade Machine either using the provided Makefile or by compiling manually. Choose the method that best suits your workflow.

### Building with Makefile

Using the Makefile allows for incremental building, only re-compiling files that have changed. This is the recommended approach for efficient development.

1. Navigate to the project directory:

    ```bash
    cd arcade-machine
    ```

2. Run the build command:

    ```bash
    make
    ```

3. Start Arcade Machine:

    ```bash
    ./ArcadeMachine
    ```

To rebuild after making code changes, simply use `make` again. For a clean build, use:

```bash
make clean
make
```

### Building Manually

If you prefer to compile without the Makefile, use the following command:

1. Compile the application:

    ```bash
    skm clang++ src/* -Iinclude -lstdc++fs -o test
    ```

2. Run the application:

    ```bash
    ./test
    ```

## Contributing

We welcome contributions to improve Arcade Machine and make it even better! If you’re interested in contributing, please review our guidelines in the [CONTRIBUTING.md](./CONTRIBUTING.md) file. Contributions can include bug fixes, new features, documentation improvements, or general enhancements. We appreciate your support in making Arcade Machine a great experience for all users!
