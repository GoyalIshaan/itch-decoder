# 📡 ITCH Market Data Decoder

A blazing fast, type-safe, and low-latency decoder for **Nasdaq TotalView-ITCH 5.0 binary protocol**, written in modern C++.

## 🚀 Performance Metrics

- **Processing Speed**: ~594,777 messages/second
- **Average Latency**: 1.68 microseconds per message
- **Tested Dataset**: Successfully processed 1.2M+ messages

---

## 🎯 Key Features

- ⚡ **Zero-Copy Memory Mapping**: Uses `mmap` for optimal file reading performance
- 🔄 **Graceful Interruption**: Handles Ctrl+C with clean statistics output
- 📊 **CSV Export**: Generates structured logs for analysis
- 🎯 **CPU Cache Optimization**: Implements prefetching for enhanced performance
- 🔍 **Real-time Monitoring**: Tracks and reports processing statistics

---

## 🧠 Project Philosophy

This project is built for **ultra-low latency** and **maximum safety**, balancing:

- ⚡ **Raw performance** (via `memcpy`-based zero-copy decoding)
- 🔐 **Type and memory safety** (via compile-time layout checks)
- 🧩 **Modularity** (message-type-specific structs in a namespace)
- 🚀 **Production-level reliability** (used in real-time feed decoding contexts)

---

## 🧱 Core Design Decisions

### ✅ 1. `PackedParser<T>` for Struct Decoding

We built a generic `PackedParser<T>` that performs a single `std::memcpy` into a pre-defined `struct`.

### Why?

- It's **much faster** than bit-by-bit manual parsing
- Prevents duplicated logic for every message type
- Great for high-throughput environments like HFT

### Safety Guards:

```cpp
static_assert(std::is_trivially_copyable<T>::value);
static_assert(std::is_standard_layout<T>::value);
```

✅ Prevents use of classes with virtual tables, dynamic memory, or complex constructors.

---

### ✅ 2. `#pragma pack(push, 1)`

Ensures **no compiler-inserted padding**.

### Why?

- Nasdaq ITCH defines **exact byte-level layouts**
- Even a single padding byte would corrupt your data

---

### ✅ 3. `static_assert(sizeof(T) == expected_size)`

Ensures **compile-time correctness** of struct layouts.

### Why?

- Protects against accidental layout shifts
- Catches bugs early in development
- Verifies ITCH compliance

---

### ✅ 4. `uint8_t timestamp[6]` for 48-bit timestamps

ITCH timestamps are 48-bit nanoseconds-since-midnight values.

### Why not `uint64_t`?

- C++ has no native `uint48_t`
- `uint8_t[6]` preserves the raw bytes safely
- Later decoding can be done via a helper

---

### ✅ 5. Fixed-Width `char[]` for Symbols and IDs

E.g., `char stock_symbol[8];`, `char reason[4];`

### Why?

- Nasdaq strings are fixed-length and space-padded
- `std::string` is dynamic, unsafe, and non-trivial (breaks `PackedParser`)
- `char[]` ensures **byte-perfect alignment and no heap allocations**

---

### ✅ 6. Namespaced Design

All structs and logic live under the `itch` namespace.

### Why?

- Avoids naming collisions
- Allows message-specific dispatch logic like `itch::parse()`
- Keeps code modular and organized

---

### ✅ 7. Dispatchable Message Type Byte

Every message struct begins with a `char message_type;`.

### Why?

- Enables fast lookup dispatch:
  ```cpp
  ParserFn parsers[256];
  parsers['R'] = [](const char* b, size_t l) { parseStockDirectoryMessage(b, l); };
  ```
- Clean separation of parsing and handling

---

## 📦 Supported Messages (so far)

| Message Type | Struct Name                        | Size | Purpose                       |
| ------------ | ---------------------------------- | ---- | ----------------------------- |
| `'S'`        | `SystemEventMessage`               | 12   | Market open/close events      |
| `'R'`        | `StockDirectoryMessage`            | 39   | Stock definitions             |
| `'H'`        | `StockTradingActionMessage`        | 25   | Halt/resume trading           |
| `'Y'`        | `RegSHOMessage`                    | 20   | Short sale rule info          |
| `'L'`        | `MarketParticipantPositionMessage` | 26   | MPID registration status      |
| `'V'`        | `MWCBDeclineLevelMessage`          | 35   | Circuit breaker thresholds    |
| `'W'`        | `MWCBStatusMessage`                | 12   | Circuit breaker breach alerts |

✅ Each struct:

- Begins with a `char message_type`
- Uses `uint8_t[6]` for timestamps
- Is `#pragma pack(1)` aligned
- Ends with `static_assert(sizeof(...))`

---

## 🛠 Future Optimizations

- SIMD for decoding batches of messages
- Arena allocators for short-lived parsed data
- Efficient timestamp converters
- C++20 concepts instead of `static_assert`
- Benchmark suite for throughput testing

---

## 💡 Why Not `reinterpret_cast`?

While faster than even `memcpy`, it:

- Is **UB** unless alignment is guaranteed
- **Fails on platforms** that don't allow strict aliasing
- Breaks on virtual classes or non-POD layouts

We chose `memcpy` + `static_assert` + `pack(1)` for **speed AND safety**.

---

## 🛠 Optimizations Implemented

- Memory-mapped I/O for zero-copy reading
- CPU cache prefetching via `__builtin_prefetch`
- Signal handling for clean interruption
- High-precision timing measurements using `CLOCK_MONOTONIC`
- Efficient binary message parsing
- CSV logging for data analysis

---

## 📊 Benchmark Results

Test conducted on real ITCH market data:

## 📂 Directory Layout (Recommended)

```

```
