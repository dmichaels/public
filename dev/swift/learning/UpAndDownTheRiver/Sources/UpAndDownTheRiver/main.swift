import Foundation

print("Hello, world!")

let command = "UpAndDownTheRiver";

let args = CommandLine.arguments;

if (args.count < 2) {
    print("usage: \(command) card");
    exit(1);
}
var verbose : Bool = false;
let intSize = MemoryLayout<Int>.size;

for i in 1..<args.count {
    let arg = args[i];
    var argi = i;
    if (arg == "--verbose") {
        verbose = true;
    }
    else if (arg == "--combinations") {
        if (argi >= args.count - 2) {
            print("usage: \(command) --combinations n r");
            exit(1);
        }
        argi += 1; let n : Int? = Int(args[argi]);
        argi += 1; let r : Int? = Int(args[argi]);
        if let n = n, let r = r {
            let c = Probability.combinations(n, r);
            print("C(n=\(n),r=\(r)): \(c)");
        }
    }
    else if (arg == "--cards") {
        if (argi >= args.count - 1) {
            print("usage: \(command) --cards cards");
            exit(1);
        }
        argi += 1;
        let cards = CardList.from(args[argi]);
        print("Cards: \(args[argi]) ...");
        for card : Card in cards.list {
            print(card.toString(verbose));
        }
        exit(0);
    }
}

var x : Card = Card(CardRank.Queen, CardSuit.Hearts);
var y : Card = Card(CardRank.Jack, CardSuit.Hearts);
print(x != y);
