class Probability {

    /// Returns the number of combinations of n items take r at a time,
    /// without doing unnecessary factorials/calculations.
    /// Recall the that formula for this is: C(n,r) = n! / (r! * (n − r)!)
    /// Stolen from: https://stackoverflow.com/questions/1838368/calculating-the-amount-of-combinations
    /// @param n Number of possible items.
    /// @param r Number of N items chosen at a time.
    /// @return Number of possible combination of n items taken r at a time.
    ///
    static func combinations(_ n: Int, _ r : Int) -> Int {
        if (r < 0 || r > n) {
            return 0;
        }
        if (r == n) {
            return 1;
        }
        var n : Int = n, k : Int = 1;
        for d in 1...r {
            k *= n;
            k /= d;
            n -= 1;
        }
        return k;
    }

    /// TODO
    /// Translating from Java to Swift ...
    ///
    /// Returns the probability that ANY of X distinct items, from a total set of N
    /// distinct items, is in a subset of R random distinct items from the set of N.
    ///
    /// Recall that the probability of EITHER of two events A or B occurring is the
    /// sum of the probabilities of either of them occurring individually, MINUS the
    /// probability that both of them will occur together; the probability of two events
    /// both occurring together is the product of the individual probabilities of each.
    /// I.e. e.g.: P(A ⋃ B) = P(A) + P(B) - P(A ∩ B) = P(A) + P(B) - (P(A) * P(B))
    ///
    /// Going from two events to N events, things get a little dicey; the first part
    /// of adding the probabilities of each of the N events individually is the same,
    /// but we need to alternatively subtract out and add in the probabilities of all
    /// combinations of the individual events occurring together. E.g. for three events:
    /// P(A ⋃ B ⋃ C) = P(C) + P(B) + P(C) - P(A ⋂ B) - P(A ⋂ C) - P(B ⋂ C) + P(A ⋂ B ⋂ C)
    /// In probability theory this is called the inclusion-exclusion principle.
    /// The pattern is in fact discernible and has been translated into the below algorithm.
    /// The number of combinations needed is done by the combinations method.
    ///
    /// The probability of any ONE of X being in R is simply R/N; the probability
    /// of BOTH of TWO of X being in R is R/N * ((R-1)/(N-1)); the probability of ALL
    /// of THREE of X being in R is R/N * ((R-1)/(N-1)) * ((R-2)/(N-2)); and so on.
    /// This part is done by the probabilityOfAll method.
    ///
    /// For reference see:
    /// https://stats.stackexchange.com/questions/97684/can-the-union-or-probability-of-many-non-mutually-exclusive-events-be-calculat
    /// https://en.wikipedia.org/wiki/Inclusion%E2%80%93exclusion_principle#In_probability
    ///
    /// There may be a simpler way to describe, understand, and compute this.
    /// Working off of musty knowledge from college probability, and Google.
    ///
    /// In our actual use case is this:
    ///
    /// N: cards in the hands of others OR in the remaining deck (i.e. not in my hand).
    /// R: cards in the hands of others.
    /// X: cards, not in my hand (i.e in N), which can beat one of my cards.
    ///
    /// Note that if there are multiple decks in a single deck and, and therefore possible
    /// duplicate cards in the total deck, it does not affect this individual calculation,
    /// as in this case, our list of cards which can beat (any) one of my cards, will
    /// contain these duplicates, since this list of cards is created from the total
    /// outstanding cards (i.e. cards not in my hand) which will therefore contain
    /// these duplicates and the calculation will be done  multiple times.
    ///
    /// @param x Number of distinct items as a subset of the set of N items.
    /// @param n Number of distinct items in the total possible set of items.
    /// @param r Number of distinct items as a subset of the set of N items.
    /// @return Probability of ANY of X items being in the R subset of N.
    ///
    static func probabilityOfAny(_ x : Int, n : Int, r : Int) -> Double {
        /*
        if ((x <= 0) || (n <= 0) || (r <= 0) || (r > n)) {
            return 0F;
        }
        var p : Double = x * probabilityOfAll(1, n, r);
        if (x >= 2) {
            for (int i = 2, sign = -1; i <= x; i++, sign *= -1) {
                let px : Double = probabilityOfAll(i, n, r);
                let c  : Double= combinations(x, i);
                p += (double)sign * (c * px);
            }
        }
        return p;
        */
        return 0;
    }

    /// TODO
    /// Translating from Java to Swift ...
    ///
    /// Returns the probability that ALL of X distinct items, from a total set of N
    /// distinct items, is in a subset of R random distinct items from the set of N.
    /// @param x Number of distinct items as a subset of the set of N items.
    /// @param n Number of distinct items in the total possible set of items.
    /// @param r Number of distinct items as a subset of the set of N items.
    /// @return Probability of ALL of X items being in the R subset of N.
    ////
    static func probabilityOfAll(_ x : Int, n : Int, r : Int) -> Double {
        /*
        if ((x <= 0) || (n <= 0) || (r <= 0) || (r > n) || (x > r)) {
            return 0F;
        }
        if (r == n) {
            return 1F;
        }
        double p = 1F;
        for (int i = 0 ; i < x ; i++) {
            p *= (double)(r - i) / (double)(n - i);
        }
        return p;
        */
        return 0;
    }

    ///
    /// TODO
    /// Translating from Java to Swift ...
    ///
    /// Returns a list of all combinations of the given list of items take R at a time.
    /// @param items The possible list of items.
    /// @param r The number of items to be taken at a time.
    /// @return List of all combinations of the given list of items.
    ////
    static func combinations<T>(items : [T], r : Int) -> [[T]]? {
        /*
        if ((items == null) || (r <= 0) || (items.size() < r)) return null;
        List<List<T>> combinations =  new ArrayList<>();
        if (r == 1) {
            for (T item : items) {
                List<T> combination = new ArrayList<>();
                combination.add(item);
                combinations.add(combination);
            }
        } else {
            final int n = items.size();
            for (int i = 0 ; i <= n - r ; i++) {
                T item = items.get(i);
                List<T> itemsRemaining = Arrays.asList(Arrays.copyOfRange(items.toArray((T[])(new Object[0])), i + 1, items.size()));
                List<List<T>> combinationsRemaining = combinations(itemsRemaining, r - 1);
                for (List<T> combination : combinationsRemaining) {
                    combination.add(0, item);
                    combinations.add(combination);
                }
            }
        }
        return combinations;
        */
        return nil;
    }
}
