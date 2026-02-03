## `Result<T,E>`
### Philosophy:
- **Explicit over Implicit**: unlike exceptions, which create hidden exit points in functions, `Result` makes the control flow transparent. By reading a function signature, you know exactly what can fail;
- **Bugs vs Errors**: We distinguish between bugs (invariant violations) and errors (expected runtime failures). Bugs should lead to an immediate stop (panic/assert), while errors are values that the program is *obligated* to handle;
- **Go Proverbs**: *"Don't just check errors, handle them gracefully"*. The eav-lib provides a functional toolkit to handle errors without nesting `if` statements.

### Algebraic Structure
`Result<T, E>` is an **Algebraic Data Type** (ADT), specifically a **Sum Type** (tagged union).

- **Mathematically**: The cardinality of `Result<T, E>` is $|T| + |E|$. This means a value is either a success type or an error type, but never both.
- **Isomorphism with Option**: 
    - `Option<T>` is a special case of `Result<T, E>` where the error type `E` carries no useful information (a unit type).
    - We can view `Option<T>` as being isomorphic to `Result<T, std::monostate>`;
    - Or we can force the erasure of type `E` in `Result<T,E>` by turning it into `Option<T>` using the `.erase_err()` method;

__Ref:__ [Logan Smith - "Result" & "Option" Isomorphism](https://youtu.be/s5S2Ed5T-dc?si=BjEAgqH6YKIdFNJG)

### Delayed Typing (Lazy Inference)
`detail::PendingType` is eav-lib feature, that simplify API usage. Below, it is written `?` instead of `detail::PendingType` for better visibility.

#### The Problem:
In standard implementations (like `std::expected`), creating a success value often requires knowing the error type immediately: `expected<int, string> res = success(10);`. This makes it difficult to use factories at the start of a pipeline.

#### The eav-lib solution:
The `make::Ok()` and `make::Err()` functions create "incomplete" `Result` objects:
- `make::Ok(10)` returns `Result<int, ?>`.
- `make::Err(std::string("error"))` returns `Result<?, std::string>`.

Guarantees: 
- `Result<T,?>` is `ok_val`;
- `Result<?,E>` is `err_val`;

These incomplete types (marked as `?`) are automatically "upgraded" to a full `Result<T, E>` as soon as the missing type is inferred from the context (e.g., during the first `AndThen`, `OrElse` call, or upon assignment).

### Combinators
| Combinator | Function Signature | Description |
| :--- | :--- | :--- |
| **MapOk** | `T -> U` | transforms the value inside `Ok`, leaving the error type unchanged |
| **AndThen** | `T -> Result<U, E>` | monadic `bind`. Chains operations that can also fail |
| **Filter** | `T -> bool` | validates the `Ok` value. If false, turns `Ok` into `Err` |
| **MapErr** | `E -> E'` | transforms the `E` without touching the `T` |
| **OrElse** | `E -> Result<T, E'>` | error recovery. Allows handling an error and returning a new `Result` |

### Pipeline Syntax
The use of the `|` operator allows for reading code from left-to-right (or top-to-bottom). This aligns with the natural flow of data in a program.

## `Option<T>`
`Option<T>` represents an optional value: every `Option` is either `Some` and contains a value, or `None`, and does not.

### Philosophy:
- **Null-safety**: replaces the dangerous use of `nullptr` or "magic constants" with a type-safe container.
- **Consistency**: shares the same Pipeline Syntax and Delayed Typing logic as `Result<T, E>`.

### Delayed Typing (Lazy Inference for None):
Guarantees: 
- `Option<?>` == `has_value()=false`

`Option<?>` is upgraded to `Option<T>` via a universal move constructor during the first interaction or assignment.

### Combinators
| Combinator | Function Signature | Description |
| :--- | :--- | :--- |
| **Map** | `T -> U` | transforms the value if present, otherwise returns `None` |
| **AndThen** | `T -> Option<U>` | monadic `bind`. chains operations that return an `Option` |
| **Filter** | `T -> bool` | keeps the value if it satisfies the predicate, otherwise returns `None` |
| **OrElse** | `void -> Option<T>` | returns fallback `Option` if the current one is `None` |

## Conversions:
- Result method **`erase_err()`**: Converts `Result<T, E>` to `Option<T>`;
- Option combinator **`OkOr(E err)`**: Converts `Option<T>` to `Result<T, E>`, using the provided error if the option has not value;
