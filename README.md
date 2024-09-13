# FT_ASYNC 🪄

This project is an asynchronous task manager written in C. It allows you to create, start, and manage asynchronous tasks.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction
The Async Task Manager is designed to help you manage asynchronous tasks in your C projects. It provides a simple interface to create and manage tasks, ensuring thread safety and efficient task execution.
## Features
- **Easy Async Management**: Create, start, and manage async tasks with a simple function.
- **Thread Safety**: Ensures safe execution and ending of tasks across multiple threads.
- **Customizable Task Functions**: Define your own start, process, and end functions for tasks.
## Getting Started

1. **Clone the repository**:
    ```sh
    git clone https://github.com/emsa001/ft_async.git
    cd ft_async
    ```

2. **Build the project**:
    ```sh
    make
    ```

3. **Run the project**:
    ```sh
    make run
    ```
## Usage

Here's a quick overview of how to use the Async Task Manager:

1. **Create a new async task**:
    ```c
    t_async *async2 = new_async();
    async2->time = 10000; // Task duration in milliseconds
    async2->start = &start;
    async2->process = &process;
    async2->process_time = 10; // Process interval in milliseconds
    async2->end = &end;
    start_async(async2);
    ```

2. **Define your task functions**:
    ```c
    void start(t_async *async) {
        printf("start thread %d\n", async->id);
    }

    void process(t_async *async) {
        int time_left = async->time - async->time_elapsed;
        printf("process thread %d: %d\n", async->id, time_left);
    }

    void end(t_async *async) {
        printf("end thread: %d\n", async->id);
    }
    ```

4. **Clean up at program exit**:
    ```c
    destroy_manager();
    ```
    
## Contributing

We welcome contributions from everyone! If you have a feature to add or a bug to fix, feel free to fork the repository and submit a pull request.
## License
This project is licensed under the MIT License. See the [MIT](https://choosealicense.com/licenses/mit/) file for details.
