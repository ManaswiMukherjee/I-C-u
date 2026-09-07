
## `char[]` — an array

```c
char resp[] = "Hello World";
```

This tells the compiler: "reserve 12 bytes of memory *right here* on the stack, and fill them with `H`,`e`,`l`,`l`,`o`,` `,`W`,`o`,`r`,`l`,`d`,`\0`."

- `resp` **is** the memory — the array itself lives wherever it's declared (stack, if local).
- The compiler knows the array is 12 bytes because it counted the characters at compile time.
- `sizeof(resp)` → `12`, because `sizeof` on an array reports the array's total byte size.
- You *can* modify the contents: `resp[0] = 'J';` is legal — you're writing into memory you own.
- You *cannot* reassign `resp` itself to point somewhere else (`resp = "Bye";` is illegal) — it's not a pointer, it's a fixed block of memory.

## `char*` — a pointer

```c
const char* send_buf = "Hello World";
```

This is a completely different thing. Here's what actually happens:

- The string literal `"Hello World"` is stored somewhere else — typically in a **read-only data section** of memory, not on the stack.
- `send_buf` is a separate variable — just a pointer — that stores the **address** of the first character (`'H'`) of that literal.
- `send_buf` itself is only as big as any pointer on your system: 8 bytes on a 64-bit machine, regardless of whether it points to a 1-character string or a 1-million-character string.
- `sizeof(send_buf)` → `8`, always — it's measuring the pointer variable, not what it points to.
- You *can* reassign `send_buf = "Different string";` — that's legal, you're just changing the address it holds.
- You generally *cannot* modify the characters through it (`send_buf[0] = 'J';` is undefined behavior / crashes on many systems) — because it points to read-only memory. That's exactly why you see `const char*` used for string literals: the `const` is documenting that reality.

## Picture it

```
char resp[] = "Hi";          const char *send_buf = "Hi";

Stack:                        Stack:
+---+---+---+                 +----------+       Read-only data:
| H | i |\0 |  <- resp IS     | send_buf |------>+---+---+---+
+---+---+---+     this data   +----------+       | H | i |\0 |
(3 bytes total)               (8 bytes,          +---+---+---+
                                just an address)
```

So the core rule: **`sizeof` on an array = the array's byte size (known at compile time). `sizeof` on a pointer = the pointer's byte size (always fixed, 4 or 8 bytes), no matter what it points to.**

If you want to know the length of the *string* a `char*` points to, you need `strlen(send_buf)` — which walks the memory byte-by-byte until it hits `\0` — not `sizeof`.
