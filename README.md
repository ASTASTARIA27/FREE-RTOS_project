Objective: To design a robust, thread-safe sensor acquisition system on a Linux-based RTOS environment.
Core Technical Features - 
1. Kernel-Level Multitasking: Leveraged the FreeRTOS POSIX Port to manage three concurrent tasks with fixed-priority preemption
2. Thread-Safe I2C Management: Implemented a Mutex Semaphore to arbitrate access to the I2C bus, preventing data collisions between the ADXL345 Accelerometer and       DS3231 RTC tasks.
3. Asynchronous Event Handling: Developed a high-priority hardware listener using libgpiod to capture falling-edge interrupts on GPIO 17, triggering a system-wide graceful shutdown.
4. Deterministic Scheduling: Replaced standard sleep calls with vTaskDelayUntil to ensure consistent 10Hz sampling frequency for telemetry data, critical for real-time analysis.
5. Resource Cleanup: Engineered a custom shutdown() sequence that ensures all RTOS tasks are deleted and Linux file descriptors (I2C/GPIO) are closed before process termination.
