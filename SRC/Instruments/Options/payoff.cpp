#include "option_type.h"
#include <cmath>
#include "payoff.h"

PlainVanillaPayoff::PlainVanillaPayoff(OptionType type, double strike)
    : type_(type), strike_(strike) 
{
}

double PlainVanillaPayoff:: operator() (double spot_) const
{
    if (type_ == OptionType::Call)
    {
        return std::max(spot_ - strike_, 0.0);
    } 
    else if (type_ == OptionType::Put)
    {
        return std::max(strike_ - spot_, 0.0);
    }
}

OptionType PlainVanillaPayoff::type() const {return type_; } // return option type Call or Put
double PlainVanillaPayoff::strike() const {return strike_; }

Payoff* PlainVanillaPayoff::clone() const
{
    return new PlainVanillaPayoff(*this);
}

// Base class = the general parent that others inherit from. Payoff is the base. 
// PlainVanillaPayoff : public Payoff — the : public Payoff means "PlainVanillaPayoff is a 
// Payoff." Payoff is the base; PlainVanillaPayoff is the derived (child).

// Abstract class = a base so incomplete you can't make one. A class is abstract if it has at 
// least one pure virtual method — one ending in = 0:
// virtual double operator()(double spot) const = 0;   // = 0 means "no body here, children MUST fill it in"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Whenever a base class has virtual Thing* clone() const = 0, that's the "copy me" button. 
// It exists because somewhere, something owns these objects through a base pointer and needs 
// to deep-copy them without knowing their real type. Every concrete subclass implements it the
//  same way: return new MySelf(*this);.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//  Explain like you're 10: the "copy me" button
// You work at a gift shop's wrapping desk. People hand you wrapped boxes. Every box has 
// the same plain sticker on the outside: "TOY." You cannot see inside. One box is really a 
// teddy bear, another a race car, another a puzzle.
// A kid runs up: "Make me an exact copy of this one!" — pointing at a box.
// You're stuck, because:
// All you see is the "TOY" sticker. You don't know if it's a bear or a car.
// You can't build "a TOY" — there's no such thing as a generic TOY. A TOY is a category, 
// not a real object. (You can only build a specific toy.)
// So the shop had a clever idea: every toy has a hidden "copy me" button. You press it, and 
// the toy itself — which of course knows what it really is — spits out an exact duplicate. 
// The teddy bear makes a teddy bear. The race car makes a race car. You never had to know 
// which was which. You just pressed "copy me," and the right copy popped out.
// That button is clone().
// The "TOY" sticker = the base type Payoff*. When you hold a payoff this way, you've "forgotten" its 
// real kind — you only see "it's a Payoff."
// The real toy inside = the actual object (a PlainVanillaPayoff).
// "You can't build a generic TOY" = Payoff is abstract; new Payoff(...) is illegal.
// Pressing "copy me" = p->clone(). The object makes its own kind, with all its own
//  guts (its strike_, its type_).
// And the trap it avoids: if you tried to copy just the sticker (the base part), you'd
//  get an empty box — you'd lose the toy inside. In C++ that real bug is called slicing: 
//  copy through the base and the derived data falls off. clone() copies the whole real toy.


// ----------------------------------------------------------------
// ----------------------------------------------------------------
// ----------------------------------------------------------------

// Mistake	Fix
// #pragma once in a .cpp	only in .h files
// constructor named Payoff::	name it after its class: PlainVanillaPayoff::PlainVanillaPayoff
// operator() missing const	match the header: add const
// used type	use the member's real name type_
// bare Call / Put	scope them: OptionType::Call / OptionType::Put
// elif	else if — and here, just else
// type() returned double	match header: return OptionType
// stray ; after {…} body	functions need no trailing ;
// clone() body missing	return new PlainVanillaPayoff(*this);
// else if with no final else	use else so all paths return
// std::max with only <cmath>	add #include <algorithm>