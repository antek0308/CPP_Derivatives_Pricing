# C++ / QuantLib-Style Pricer — Personal Playbook

> Goal of this file: let future-me rebuild this kind of architecture **10× faster**.
> Read top-to-bottom once; after that, jump to **Templates** and **"When I see X → do Y"**.

---

## 1. The one big idea (mental model)

**An option doesn't know how to price itself. You bolt a *pricing engine* onto it and ask for the price.**
Swap the engine → same option, different method (formula / simulation / PDE). This is the **Strategy pattern**.

```
   OPTION (the "order")  ──setPricingEngine(engine)──►  holds an engine in a slot
        │
        └── NPV()  ("make my order")  ──►  engine->calculate(*this)  ("the cooking")  ──►  price
```

- **Instrument** = the *noun* (what the contract is). Has a `NPV()` button.
- **Engine** = the *verb* (how to price). Has `calculate()`, the worker.
- **Payoff / Process** = the data the engine reads (contract terms + market).

> Analogy that unstuck me: **option = food order, engine = chef.** `NPV()` = "make my order", `calculate()` = the actual cooking. The order doesn't care which chef cooks it.

---

## 2. The layer cake (what lives where)

```
Payoff            (abstract)   operator()(spot)  + strike()/type()      ← "rich": serves ALL engines
   └─ PlainVanillaPayoff
Process           BlackScholesProcess: spot, r, d, vol                  ← the market   [Heston later]
Instrument        (base)   setPricingEngine(), NPV()
   └─ EuropeanOption: holds shared_ptr<Payoff> + expiry                 [BarrierOption later]
PricingEngine     (abstract)   calculate(const Instrument&) = 0         ← the "strategy" contract
   ├─ AnalyticEuropeanEngine   reads strike/type → bs_price formula
   └─ MonteCarloEngine         simulates → averages payoff(S_T)         [FD/PDE engine later]
```

**Folder map (QuantLib-style, by concept):**
`Core/` (math util) · `Processes/` (dynamics) · `Instruments/` (+`Options/`) · `Engines/` (`Analytic`, `MC`, `FDM`) · `Statistics/` · `TermStructures/` (curves / local vol).

---

## 3. Templates (copy these — they are the whole project's DNA)

### 3a. A plain data holder (value type) — e.g. a Process
```cpp
// foo.h
#pragma once
class Foo {
public:
    Foo(double a, double b);
    double a() const;          // accessors are const, return the member
    double b() const;
private:
    double a_;                 // members get a trailing underscore
    double b_;
};
```
```cpp
// foo.cpp
#include "foo.h"
Foo::Foo(double a, double b) : a_(a), b_(b) {}   // init list order MUST match member order
double Foo::a() const { return a_; }
double Foo::b() const { return b_; }
```

### 3b. An abstract base + concrete child (polymorphism)
```cpp
// payoff.h
#pragma once
#include "option_type.h"                 // enum class OptionType { Call, Put };
class Payoff {                            // ABSTRACT: has a "= 0" method
public:
    virtual double operator()(double spot) const = 0;   // every child MUST implement
    virtual ~Payoff() {}
};
class PlainVanillaPayoff : public Payoff {              // "is-a" Payoff
public:
    PlainVanillaPayoff(OptionType type, double strike);
    double     operator()(double spot) const override;  // override = "I implement the base's virtual"
    OptionType type()   const;                          // extra accessors (introspection)
    double     strike() const;
private:
    OptionType type_;
    double     strike_;
};
```
```cpp
// payoff.cpp
#include "payoff.h"
#include <algorithm>
PlainVanillaPayoff::PlainVanillaPayoff(OptionType t, double k) : type_(t), strike_(k) {}
double PlainVanillaPayoff::operator()(double s) const {
    if (type_ == OptionType::Call) return std::max(s - strike_, 0.0);
    else                           return std::max(strike_ - s, 0.0);   // use 'else' so every path returns
}
OptionType PlainVanillaPayoff::type()   const { return type_; }
double     PlainVanillaPayoff::strike() const { return strike_; }
```

### 3c. Instrument base + a concrete instrument
```cpp
// instrument.h
#pragma once
#include <memory>
class PricingEngine;                       // forward decl (only used as shared_ptr → no #include)
class Instrument {
public:
    void   setPricingEngine(std::shared_ptr<PricingEngine> e);
    double NPV() const;                    // body in .cpp (it USES the engine)
    virtual ~Instrument() {}
private:
    std::shared_ptr<PricingEngine> engine_;
};
```
```cpp
// instrument.cpp
#include "instrument.h"
#include "pricing_engine.h"                // full include HERE (we use calculate)
void   Instrument::setPricingEngine(std::shared_ptr<PricingEngine> e) { engine_ = e; }
double Instrument::NPV() const { return engine_->calculate(*this); }   // delegate to the engine
```
```cpp
// european_option.h
#pragma once
#include "payoff.h"
#include "instrument.h"
#include <memory>
class EuropeanOption : public Instrument {
public:
    EuropeanOption(std::shared_ptr<Payoff> payoff, double expiry);
    double        expiry() const;
    const Payoff& payoff() const;          // returns *payoff_  (deref the shared_ptr)
private:
    std::shared_ptr<Payoff> payoff_;
    double expiry_;
};
```

### 3d. The engine: abstract base + a concrete engine
```cpp
// pricing_engine.h
#pragma once
class Instrument;                          // forward decl
class PricingEngine {
public:
    virtual double calculate(const Instrument& i) const = 0;   // the contract
    virtual ~PricingEngine() {}
};
```
```cpp
// analytic_european_engine.h
#pragma once
#include "pricing_engine.h"
#include "black_scholes_process.h"
class AnalyticEuropeanEngine : public PricingEngine {
public:
    AnalyticEuropeanEngine(BlackScholesProcess process);
    double calculate(const Instrument& i) const override;
private:
    BlackScholesProcess process_;          // the engine carries its market
};
```
```cpp
// analytic_european_engine.cpp  (the heart)
double AnalyticEuropeanEngine::calculate(const Instrument& i) const {
    const EuropeanOption&     opt = dynamic_cast<const EuropeanOption&>(i);          // unlock option fields
    const PlainVanillaPayoff& pay = dynamic_cast<const PlainVanillaPayoff&>(opt.payoff()); // unlock strike/type
    int call = (pay.type() == OptionType::Call) ? 1 : 0;
    return bs_price(process_.spot(), pay.strike(), process_.r(), process_.d(),
                    process_.vol(), opt.expiry(), call);                              // market + contract → formula
}
```

### 3e. The usage (the "see the end" snippet)
```cpp
auto payoff = std::make_shared<PlainVanillaPayoff>(OptionType::Call, 110);
EuropeanOption option(payoff, 0.5);
BlackScholesProcess process(110, 0.23, 0.05, 0.0);          // (spot, vol, r, d)

option.setPricingEngine(std::make_shared<AnalyticEuropeanEngine>(process));
std::cout << option.NPV();                                  // 8.4819  (formula)

option.setPricingEngine(std::make_shared<MonteCarloEngine>(process, 1'000'000));
std::cout << option.NPV();                                  // ~8.478  (simulation) — SAME option
```

### 3f. Monte-Carlo body (European, terminal-only)
```cpp
double MonteCarloEngine::calculate(const Instrument& i) const {
    const EuropeanOption& opt = dynamic_cast<const EuropeanOption&>(i);
    const Payoff& payoff = opt.payoff();                 // NO downcast — MC only EVALUATES the payoff
    double S0=process_.spot(), r=process_.r(), d=process_.d(), vol=process_.vol(), T=opt.expiry();
    double drift = (r - d - 0.5*vol*vol) * T;            // ← note the *T
    double diff  = vol * std::sqrt(T);
    std::mt19937_64 rng(12345);
    std::normal_distribution<double> Z(0.0, 1.0);
    double sum = 0.0;
    for (unsigned long n = 0; n < number_of_paths_; ++n) {
        double ST = S0 * std::exp(drift + diff * Z(rng));
        sum += payoff(ST);
    }
    return std::exp(-r*T) * (sum / number_of_paths_);    // discount with r ONLY
}
```

---

## 4. "When I see X → do Y" (fast-debug)

### Compiler / build errors

**`X does not name a type` / `unknown type name X`**
→ Compiler hasn't seen `X` yet. `#include "x.h"`, or a **forward decl** `class X;` if you only use a reference/pointer.

**`'foo.h' file not found`**  *(editor only — g++ builds fine)*
→ IntelliSense doesn't know your `-I` paths. Cosmetic; ignore it (or add paths to `c_cpp_properties.json`).

**`undefined reference to X`**  *(linker error)*
→ The `.h` was found, but the `.cpp` holding the **body** wasn't compiled. Add that `.cpp` to the build / Makefile `SRCS`.

**`no declaration matches 'Foo::bar(...)'`**
→ Your `.cpp` signature ≠ the `.h` signature. Make them **identical**: name, params, order, `const`, return type.

**`multiple definition of X`**
→ Defined twice in one file. One definition only; add `#pragma once`; don't redefine an enum you already `#include`.

**`-Wreorder`**
→ Init-list order ≠ member-declaration order. Reorder the init list to match how members are declared.

**`control reaches end of non-void function`**
→ A branch doesn't `return`. Use `else` (not `else if`) so every path returns a value.

**`-Wsign-compare`**
→ Comparing `int` with `unsigned`. Make the loop counter `unsigned long`.

**`override` rejected in the `.cpp`**
→ `override` goes only on the **declaration**. Keep it in the `.h`, remove it from the `.cpp`.

### Design decisions

- **Hold a `Base&` but need a derived field** (`strike`, `payoff`…) → `dynamic_cast<const Derived&>(base)`, and *only* then.
- **The method is already on the base type you hold** → no cast, just call it.
- **Store a polymorphic object** (abstract base) → `std::shared_ptr<Base>` member (never `Base` by value — it's abstract, and copying *slices*).
- **Let callers read a field but not change it** → private member + `const` accessor.
- **A method changes no member** → mark it `const`.
- **Two headers need each other** → forward-declare in the `.h`, full `#include` in the `.cpp`.
- **Just bundling some numbers** → the §3a data-holder template.
- **Behavior that varies by subtype** → abstract base + `virtual … = 0` + children (§3b template).

> **The one rule that prevents most pain:**
> A definition in the `.cpp` is a *carbon copy* of its declaration in the `.h` (minus the `;`).
> **Header = the menu. `.cpp` = the kitchen.**

---

## 5. How to EXTEND this architecture (recipes)

**Add a new payoff** (digital, power…)
→ New class `: public Payoff`; implement `operator()` (+ `strike()`/`type()`-style accessors *only* if a formula engine needs them). Any engine that only *evaluates* (MC, PDE) works with it instantly.

**Add a new instrument** (Barrier, Asian)
→ New class `: public Instrument` with its extra fields (barrier level, up/down, knock in/out). It gets `NPV()` and `setPricingEngine()` for free.

**Add a new pricing engine** (PDE, tree)
→ New class `: public PricingEngine`; implement `calculate()`. Attach it to the *same* instruments — that's the whole point.

**Add a new market model** (Heston, local vol)
→ New `Process` (e.g. `HestonProcess`); simulating engines read the process instead of hardcoding GBM. Local vol = replace the scalar `vol` in the process with a surface (lives in `TermStructures/`).

**Price something path-dependent** (barriers!)
→ The MC engine must simulate a **multi-step path** (loop over time) and check the condition each step — not the single terminal jump Europeans use. Reference: old `path_dependent_barrier_asian` + `exotic_bs_engine`.

**Add statistics** (std error, CI)
→ Feed each path's discounted payoff into a gatherer; report mean ± standard error and a confidence interval.

**Naming convention (QuantLib):** `<Method><Model><Product>Engine`
e.g. `FdHestonBarrierEngine` = finite-difference + Heston + barrier.

---

## 6. Flashcards (cover the answer line)

**Q — What makes a class *abstract*?**
It has ≥1 pure-virtual (`= 0`) method → you can't instantiate it.

**Q — Why does a base pointer need `dynamic_cast`?**
Through the base you only see base methods; the cast unlocks the derived type's fields.

**Q — `.h` vs `.cpp`?**
`.h` = declarations (the menu, `#include`d everywhere). `.cpp` = bodies (the kitchen, compiled once).

**Q — When is a forward declaration enough?**
When you only use the type as a reference/pointer. Need a full `#include` to use members, store by value, or inherit.

**Q — Why `shared_ptr<Payoff>` not `Payoff`?**
`Payoff` is abstract (can't instantiate) and copying into a base slot *slices*; the smart pointer also gives Rule-of-Zero memory management.

**Q — `clone()` — what & why?**
A "copy-me" button for polymorphic value semantics; needed to deep-copy through a base pointer. (Avoided entirely by `shared_ptr`.)

**Q — Where does `override` go?**
On the declaration in the `.h`, never the `.cpp` definition.

**Q — Init-list order rule?**
Members initialize in *declaration order* no matter how you write the list — keep them matching to avoid `-Wreorder`.

**Q — `NPV()` vs `calculate()`?**
`NPV()` is the instrument's button → it delegates. `calculate()` is the engine's actual math.

**Q — MC: why no payoff downcast, but analytic needs one?**
MC only *evaluates* the payoff (`operator()`, on the base). Analytic needs *strike/type* (only on the concrete) → cast.

**Q — European MC terminal price?**
`S_T = S0·exp((r−d−½σ²)T + σ√T·Z)`, `Z~N(0,1)`; price = `e^(−rT) · mean(payoff(S_T))`.

**Q — Build command on this machine?**
`mingw32-make run` (PowerShell; there's no `make`; `\` line-continuation doesn't work there).

---

## 7. What changed (Joshi → QuantLib-style)

- `Wrapper<T>` (hand-rolled, clones)  →  **`std::shared_ptr<T>`** — modern, standard, Rule-of-Zero (what QuantLib uses).
- `PayOff` (evaluate only)  →  **`Payoff`** (evaluate **+** introspect) — one payoff serves every engine.
- `VanillaOption` (flat data)  →  **`EuropeanOption : Instrument`** (holds a Payoff) — composition; one instrument, many engines.
- engine built around the product  →  **`instrument.setPricingEngine(e); NPV()`** — the Strategy pattern, swap engines freely.
- `PathDependent` + cashflow gatherers  →  **kept as reference** for the barrier work — clean rewrite beats untangling.
- flat folder of ~40 files  →  **`SRC/` by concept** (QuantLib `ql/` style) — obvious homes for PDE / Heston / local vol.

---

### Next session starts at: **statistics gatherer** (mean + std error + CI in MC), then **barrier options** (the deliverable).
