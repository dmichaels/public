from __future__ import annotations
from collections import Counter
from enum import Enum
from itertools import combinations
import random
import re
from typing import Callable, List, Optional, Union


class Chars:
    SPADES = "♠"
    HEARTS = "♥"
    DIAMONDS = "♦"
    CLUBS = "♣"
    DOT = "·"
    RARROW = "➤"
    THINSPACE = "\u200a"


class Rank(Enum):

    ACE = 1 
    TWO = 2
    THREE = 3
    FOUR = 4
    FIVE = 5
    SIX = 6
    SEVEN = 7
    EIGHT = 8
    NINE = 9
    TEN = 10
    JACK = 11
    QUEEN = 12
    KING = 13

    @staticmethod
    def create(value: Union[int, str, Rank]) -> Optional[Rank]:
        if isinstance(value, Rank):
            return value
        elif isinstance(value, int):
            if value in Rank.values():
                return Rank(value)
            elif value in Rank.values_ace_high():
                return Rank.ACE
        elif isinstance(value, str) and (value := value.strip().upper()):
            if value in ["JACK", "J"]: return Rank.JACK
            elif value in ["QUEEN", "Q"]: return Rank.QUEEN
            elif value in ["KING", "K"]: return Rank.KING
            elif value in ["ACE", "A"]: return Rank.ACE
            elif value.isdigit():
                if (value := int(value)) in Rank.values():
                    return Rank(value)
                elif value in Rank.values_ace_high():
                    return Rank.ACE
        return None

    @staticmethod
    def values() -> List[int]:
        return [rank.value for rank in Rank]

    @staticmethod
    def values_ace_high() -> List[int]:
        return [rank.value for rank in Rank if rank.value > 1] + [14]

    @property
    def value_ace_high(self) -> int:
        return 14 if (self == Rank.ACE) else self.value 

    def __str__(self) -> str:
        match self:
            case Rank.ACE: return "A"
            case Rank.TWO: return "2"
            case Rank.THREE: return "3"
            case Rank.FOUR: return "4"
            case Rank.FIVE: return "5"
            case Rank.SIX: return "6"
            case Rank.SEVEN: return "7"
            case Rank.EIGHT: return "8"
            case Rank.NINE: return "9"
            case Rank.TEN: return "10"
            case Rank.JACK: return "J"
            case Rank.QUEEN: return "Q"
            case Rank.KING: return "K"
            case _: return ""


class Suit(Enum):

    CLUBS = 1
    DIAMONDS = 2
    HEARTS = 3
    SPADES = 4 

    @staticmethod
    def create(value: Union[int, str, Suit]) -> Optional[Suit]:
        if isinstance(value, Suit):
            return value
        elif isinstance(value, int):
            if value in Suit.values():
                return Suit(value)
        elif isinstance(value, str) and (value := value.strip().upper()):
            if value in ["CLUBS", "CLUB", "C", Chars.CLUBS]: return Suit.CLUBS
            elif value in ["DIAMONDS", "DIAMOND", "D", Chars.DIAMONDS]: return Suit.DIAMONDS
            elif value in ["HEARTS", "HEARTS", "H", Chars.HEARTS]: return Suit.HEARTS
            elif value in ["SPADES", "SPADES", "S", Chars.SPADES]: return Suit.SPADES
        return None

    @staticmethod
    def values() -> List[int]:
        return [suit.value for suit in Suit]

    def __str__(self) -> str:
        match self:
            case Suit.SPADES: return Chars.SPADES
            case Suit.HEARTS: return Chars.HEARTS
            case Suit.DIAMONDS: return Chars.DIAMONDS
            case Suit.CLUBS: return Chars.CLUBS
            case _: return ""


class Card:

    def __init__(self, rank: Union[str, Rank, Suit], suit: Optional[Union[str, Suit, Rank]] = None) -> None:
        rank, suit = Card.create(rank, suit, _internal=True)
        if (rank is not None) and (suit is not None):
            self._rank = rank
            self._suit = suit
        else:
            self._rank = Rank.TWO
            self._suit = Suit.CLUBS

    @staticmethod
    def create(rank: Union[str, Rank, Suit], suit: Optional[Union[str, Suit, Rank]] = None,
               _internal: bool = False) -> Union[Optional[Card], Tuple[Optional[Rank], Optional[Suit]]]:
        if ((r := Rank.create(rank)) is not None) and ((s := Suit.create(suit)) is not None):
            return Card(r, s) if _internal is not True else (r, s)
        elif ((r := Rank.create(suit)) is not None) and ((s := Suit.create(rank)) is not None):
            return Card(r, s) if _internal is not True else (r, s)
        elif isinstance(rank, str) and suit is None:
            string = rank
            if isinstance(string, str) and (string := string.strip().replace("-","").upper()):
                def check_string(substrings: List[str]) -> bool:
                    nonlocal string
                    for substring in substrings:
                        if substring in string:
                            string = string.replace(substring, "").strip()
                            return True
                    return False
                if check_string(["CLUBS", "CLUB", Chars.CLUBS]): suit = Suit.CLUBS
                elif check_string(["DIAMONDS", "DIAMOND", Chars.DIAMONDS]): suit = Suit.DIAMONDS
                elif check_string(["HEARTS", "HEART", Chars.HEARTS]): suit = Suit.HEARTS
                elif check_string(["SPADES", "SPADE", Chars.SPADES]): suit = Suit.SPADES
                elif check_string(["C"]): suit = Suit.CLUBS
                elif check_string(["D"]): suit = Suit.DIAMONDS
                elif check_string(["H"]): suit = Suit.HEARTS
                elif check_string(["S"]): suit = Suit.SPADES
                else: return None if _internal is not True else (None, None)
                if check_string(["JACK"]): rank = Rank.JACK
                elif check_string(["QUEEN"]): rank = Rank.QUEEN
                elif check_string(["KING"]): rank = Rank.KING
                elif check_string(["ACE"]): rank = Rank.ACE
                elif check_string(["J"]): rank = Rank.JACK
                elif check_string(["Q"]): rank = Rank.QUEEN
                elif check_string(["K"]): rank = Rank.KING
                elif check_string(["A"]): rank = Rank.ACE
                elif string.isdigit():
                    if ((rank := int(string)) not in Rank.values()) and (rank not in Rank.values_ace_high()):
                        return None if _internal is not True else (None, None)
                    rank = Rank(rank)
                else: return None if _internal is not True else (None, None)
                return Card(rank, suit) if _internal is not True else (rank, suit)
            return None if _internal is not True else (None, None)
        return None if _internal is not True else (None, None)

    def copy(self) -> Card:
        return Card(self._rank, self._suit)

    @property
    def rank(self) -> Rank:
        return self._rank

    @property
    def suit(self) -> Suit:
        return self._suit

    def __eq__(self, other: Card) -> bool:
        return isinstance(other, Card) and (other._rank == self._rank) and (other._suit == self._suit)

    def __ne__(self, other: Card) -> bool:
        return not self._eq__(other)

    def __hash__(self):
        return hash((self._rank, self._suit))

    def __str__(self):
        return f"{self._rank}{Chars.THINSPACE}{self._suit}"


class Cards(List[Card]):

    def __init__(self, *args) -> None:
        self.set(*args)

    @staticmethod
    def create(string: str, nodups: bool = False) -> Cards:
        cards = Cards()
        if isinstance(string, str):
            values = re.split(f"[{re.escape(' ,;|/')}]", string)
            for value in values:
                if card := Card.create(value):
                    if (nodups is not True) or (not cards.contains(card)):
                        cards.add(card)
        return cards

    def copy(self) -> Cards:
        return Cards(list(self))

    @property
    def count(self) -> int:
        return len(self)

    def set(self, *args) -> None:
        self.clear()
        self.add(*args)

    def add(self, *args, nodups: bool = False) -> None:
        for arg in args:
            if isinstance(arg, Card):
                if (nodups is not True) or (not self.contains(arg)):
                    super().append(arg)
            elif isinstance(arg, (list, set, tuple)):
                self.add(*arg)

    def contains(self, card: Card) -> bool:
        return isinstance(card, Card) and (card in self)

    def remove(self, card: Card, all: bool = False) -> bool:
        if isinstance(card, Card):
            if all is True:
                ncards = 0
                while True:
                    if card not in self:
                        break
                    super().remove(card)
                    ncards += 1
                return ncards > 0
            elif card in self:
                super().remove(card)
                return True
        return False

    def peek_top(self, ncards: Optional[int] = None) -> Union[Optional[Card], List[Cards]]:
        if isinstance(ncards, int):
            cards = Cards()
            if ncards > 1:
                for n in range(ncards):
                    if not (card := self.peek_at(0)):
                        break
                    cards.add(card)
            return cards
        return self.peek_at(0)

    def peek_random(self, ncards: Optional[int] = None) -> Union[Optional[Card], List[Cards]]:
        if isinstance(ncards, int):
            cards = Cards()
            if ncards > 1:
                for n in range(ncards):
                    if not (card := self.peek_at(None)):
                        break
                    cards.add(card)
            return cards
        return self.peek_at(None)

    def peek_at(self, index: Optional[int] = None) -> Optional[Card]:
        if (index := self._index(index)) is not None:
            return self[index]
        return None

    def take_top(self, ncards: Optional[int] = None) -> Union[Optional[Card], List[Cards]]:
        if isinstance(ncards, int):
            cards = Cards()
            if ncards > 1:
                for n in range(ncards):
                    if not (card := self.take_at(0)):
                        break
                    cards.add(card)
            return cards
        return self.take_at(0)

    def take_random(self, ncards: Optional[int] = None) -> Union[Optional[Card], List[Cards]]:
        if isinstance(ncards, int):
            cards = Cards()
            if ncards > 1:
                for n in range(ncards):
                    if not (card := self.take_at(None)):
                        break
                    cards.add(card)
            return cards
        return self.take_at(None)

    def take_at(self, index: Optional[int] = None) -> Optional[Card]:
        if (index := self._index(index)) is not None:
            card = self[index]
            del self[index]
            return card
        return None

    def untake(self, card: Card, index: Optional[int] = None) -> bool:
        if isinstance(card, Card):
            if isinstance(index, int) and (index >= 0) and (index <= len(self)):
                self.insert(index, card)
            else:
                self.add(card)
            return True
        return False

    def _index(self, index: Optional[int] = None) -> Optional[int]:
        if (ncards := len(self)) > 0:
            if index is None:
                return random.randint(0, ncards - 1)
            elif isinstance(index, int) and (index >= 0) and (index < ncards):
                return index
        return None

    def _random(self) -> Optional[int]:
        return self._index(None)

    def _randomize(self) -> None:
        random.shuffle(self)

    def __add__(self, other: Cards) -> PokerHand:
        return Cards(super().__add__(other)) if isinstance(other, Cards) else Cards(self)

    def __str__(self) -> str:
        string = " ".join(f"{str(card):>5}" for card in self)
        if string.startswith(" "):
            string = string[1:]
        return string


class Deck(Cards):

    def __init__(self, *args, shuffle: bool = True) -> None:
        if len(args) == 0:
            super().__init__(Deck._create_cards())
        else:
            super().__init__(*args)
        if shuffle is not False:
            self.shuffle()

    @staticmethod
    def create(*args, shuffle: bool = True) -> List[Card]:
        return Deck(*args, shuffle=shuffle)

    def copy(self) -> Deck:
        return Deck(list(self))

    def shuffle(self) -> None:
        super()._randomize()

    @staticmethod
    def _create_cards() -> List[Card]:
        cards = []
        for suit in Suit:
            for rank in Rank:
                cards.append(Card(rank, suit))
        return cards


class PokerHand(Cards):

    NCARDS = 5

    def __init__(self, *args) -> None:
        super().__init__(*args)

    @staticmethod
    def create(deck: Optional[Deck] = None, ncards: Optional[int] = None):
        if isinstance(deck, str):
            return PokerHand(Cards.create(deck))
        elif isinstance(deck, int) and isinstance(ncards, Cards):
            deck, ncards = ncards, deck
        else:
            if not isinstance(deck, Cards):
                deck = Deck()
            if not (isinstance(ncards, int) and (ncards > 0)):
                ncards = PokerHand.NCARDS
        hand = PokerHand()
        for n in range(ncards):
            if not (card := deck.take_random()):
                break
            hand.add(card)
        return hand

    def copy(self) -> PokerHand:
        return PokerHand(list(self))

    def fill(self, deck: Deck, ncards: Optional[int] = None) -> int:
        if isinstance(deck, int) and isinstance(ncards, Cards):
            deck, ncards = ncards, deck
        else:
            if not isinstance(deck, Cards):
                deck = Deck()
            if not (isinstance(ncards, int) and (ncards > 0)):
                ncards = PokerHand.NCARDS
        nadded = 0
        if self.count < ncards:
            for n in range(ncards - self.count):
                if not (card := deck.take_random()):
                    break
                self.add(card)
                nadded += 1
        return nadded

    def sorted(self, reverse: bool = False) -> PokerHand:
        return PokerHand(sorted(self, key=lambda card: (-card.suit.value, card.rank.value), reverse=reverse is True))

    def evaluate(self) -> Tuple[int, List[int]]:
        return PokerHandCategory.evaluate_hand(self)

    def evaluate_category(self) -> PokerHandCategory:
        return PokerHandCategory.evaluate_hand_category(self)

    def has_royal_straight_flush(self) -> bool:
        return PokerHandCategory.is_royal_straight_flush(self)

    def has_straight_flush(self) -> bool:
        return PokerHandCategory.is_straight_flush(self)

    def has_four_of_a_kind(self) -> bool:
        return PokerHandCategory.is_four_of_a_kind(self)

    def has_full_house(self) -> bool:
        return PokerHandCategory.is_full_house(self)

    def has_flush(self) -> bool:
        return PokerHandCategory.is_flush(self)

    def has_straight(self) -> bool:
        return PokerHandCategory.is_straight(self)

    def has_three_of_a_kind(self) -> bool:
        return PokerHandCategory.is_three_of_a_kind(self)

    def has_two_pair(self) -> bool:
        return PokerHandCategory.is_two_pair(self)

    def has_one_pair(self) -> bool:
        return PokerHandCategory.is_one_pair(self)

    def __add__(self, other: Cards) -> PokerHand:
        return PokerHand(super().__add__(other)) if isinstance(other, Cards) else PokerHand(self)

    @staticmethod
    def get_possible_hands(cards: Cards, ncards: Optional[int] = None) -> List[PokerHand]:
        if isinstance(cards, int) and isinstance(ncards, (Cards, list, set, tuple)):
            cards, ncards = ncards, cards
        else:
            if not (isinstance(ncards, int) and (ncards > 0)):
                ncards = PokerHand.NCARDS
        if isinstance(cards, (list, set, tuple)):
            cards = Cards(cards)
        elif not isinstance(cards, Cards):
            return []
        if cards.count < ncards:
            ncards = cards.count
        return [PokerHand(hand) for hand in combinations(cards, ncards)]

    @staticmethod
    def visit_possible_hands(callback: Optional[Callable] = None, ncards: Optional[int] = None,
                             deck: Optional[Cards] = None, ordered: bool = False) -> None:
        #
        # total                 ·➤ 2598960 · 100.0000%
        # high card             ·➤ 1302540 ·  50.1177%
        # one pair              ·➤ 1098240 ·  42.2569%
        # two pair              ·➤  123552 ·   4.7539%
        # three of a kind       ·➤   54912 ·   2.1128%
        # straight              ·➤   10200 ·   0.3925%
        # flush                 ·➤    5108 ·   0.1965%
        # full house            ·➤    3744 ·   0.1441%
        # four of a kind        ·➤     624 ·   0.0240%
        # straight flush        ·➤      36 ·   0.0014%
        # royal straight flush  ·➤       4 ·   0.0002%
        #
        def possible_hands(deck: Cards, ncards: int, callback: Callable, cards: Optional[Cards] = None) -> None:
            nonlocal ordered
            if ncards == 1:
                for card in deck:
                    callback(PokerHand(cards, card))
                return
            if ordered is True:
                for index in range(deck.count):
                    card = deck.take_at(index)
                    possible_hands(deck=deck, ncards=ncards - 1, callback=callback, cards=Cards(cards, card))
                    deck.untake(card, index)
            else:
                for card in deck.copy():
                    deck.remove(card)
                    possible_hands(deck=deck.copy(), ncards=ncards - 1, callback=callback, cards=Cards(cards, card))
        if not isinstance(deck, Cards):
            deck = Deck()
        if not (isinstance(ncards, int) and (ncards > 0)):
            ncards = PokerHand.NCARDS
        if isinstance(deck, Cards) and isinstance(ncards, int) and (ncards > 0):
            if not callable(callback):
                callback = lambda hand: print(hand)
            possible_hands(deck=deck.copy(), ncards=ncards, callback=callback)

    def to_string(self, sort: bool = True, verbose: bool = True) -> str:
        hand = self.sorted() if sort is not False else self
        hand_category = hand.evaluate_category() if verbose is not False else None
        suffix = f"  {Chars.DOT}{Chars.RARROW} {hand_category}" if hand_category else ""
        return f"{hand}{suffix}"


class PokerHandCategory(Enum):

    HIGH_CARD = 1
    ONE_PAIR = 2
    TWO_PAIR = 3
    THREE_OF_A_KIND = 4
    STRAIGHT = 5
    FLUSH = 6
    FULL_HOUSE = 7
    FOUR_OF_A_KIND = 8
    STRAIGHT_FLUSH = 9
    ROYAL_STRAIGHT_FLUSH = 10

    @staticmethod
    def evaluate_hands(hands: List[Cards]) -> List[Cards]:
        ranked_hands = []
        if isinstance(hands, (list, set, tuple)):
            ranked_hands = sorted([hand for hand in hands if isinstance(hand, Cards)],
                                  key=lambda hand: PokerHandCategory.evaluate_hand(hand), reverse=True)
        return ranked_hands

    @staticmethod
    def evaluate_hand(hand: Cards) -> Tuple[int, List[int]]:
        """
        Evaluates the strength of the given (five-card poker) PokerHand and returns a tuple with its
        PokerHandCategory (as an integer), and relevant ranks (list of integers) suitable for tie-breaking.
        N.B. Adapted from ChatGPT.
        """
        rank_values = sorted([card.rank.value_ace_high for card in hand], reverse=True)
        rank_counter = Counter(rank_values)
        rank_count_items = rank_counter.items()
        rank_counts = sorted(rank_counter.values(), reverse=True)
        rank_uniques = sorted(set(rank_values), reverse=True)
        suits = [card.suit for card in hand]
        is_flush = (len(set(suits)) == 1)
        is_straight = False
        high_card_straight = None
        if len(rank_uniques) == 5:
            if (rank_uniques[0] - rank_uniques[4]) == 4:
                is_straight = True
                high_card_straight = rank_uniques[0]
            elif rank_uniques == [Rank.ACE.value_ace_high, Rank.FIVE.value,
                                  Rank.FOUR.value, Rank.THREE.value, Rank.TWO.value]:
                is_straight = True
                high_card_straight = Rank.FIVE.value
        if is_straight and is_flush:
            if high_card_straight == Rank.ACE.value_ace_high:
                category = PokerHandCategory.ROYAL_STRAIGHT_FLUSH
            else:
                category = PokerHandCategory.STRAIGHT_FLUSH
        elif rank_counts[0] == 4:
            category = PokerHandCategory.FOUR_OF_A_KIND
        elif (rank_counts[0] == 3) and (rank_counts[1] == 2):
            category = PokerHandCategory.FULL_HOUSE
        elif is_flush:
            category = PokerHandCategory.FLUSH
        elif is_straight:
            category = PokerHandCategory.STRAIGHT
        elif rank_counts[0] == 3:
            category = PokerHandCategory.THREE_OF_A_KIND
        elif (rank_counts[0] == 2) and (rank_counts[1] == 2):
            category = PokerHandCategory.TWO_PAIR
        elif rank_counts[0] == 2:
            category = PokerHandCategory.ONE_PAIR
        else:
            category = PokerHandCategory.HIGH_CARD
        if category == PokerHandCategory.ROYAL_STRAIGHT_FLUSH:
            tie_breaker = [Rank.ACE.value_ace_high]
        elif category == PokerHandCategory.STRAIGHT_FLUSH or category == PokerHandCategory.STRAIGHT:
            tie_breaker = [high_card_straight]
        elif category == PokerHandCategory.FOUR_OF_A_KIND:
            four_rank = [rank for rank, count in rank_count_items if count == 4][0]
            kicker = max([rank for rank in rank_values if rank != four_rank])
            tie_breaker = [four_rank, kicker]
        elif category == PokerHandCategory.FULL_HOUSE:
            three_rank = [rank for rank, count in rank_count_items if count == 3][0]
            pair_rank = [rank for rank, count in rank_count_items if count == 2][0]
            tie_breaker = [three_rank, pair_rank]
        elif category == PokerHandCategory.FLUSH:
            tie_breaker = sorted(rank_values, reverse=True)
        elif category == PokerHandCategory.STRAIGHT:
            tie_breaker = [high_card_straight]
        elif category == PokerHandCategory.THREE_OF_A_KIND:
            three_rank = [rank for rank, count in rank_count_items if count == 3][0]
            kickers = sorted([rank for rank in rank_values if rank != three_rank], reverse=True)
            tie_breaker = [three_rank] + kickers
        elif category == PokerHandCategory.TWO_PAIR:
            pairs = sorted([rank for rank, count in rank_count_items if count == 2], reverse=True)
            kicker = max([rank for rank in rank_values if rank not in pairs])
            tie_breaker = pairs + [kicker]
        elif category == PokerHandCategory.ONE_PAIR:
            pair_rank = [rank for rank, count in rank_count_items if count == 2][0]
            kickers = sorted([rank for rank in rank_values if rank != pair_rank], reverse=True)
            tie_breaker = [pair_rank] + kickers
        else:
            tie_breaker = sorted(rank_values, reverse=True)
        return (category.value, tie_breaker)

    @staticmethod
    def evaluate_hand_category(hand: Cards) -> PokerHandCategory:
        if PokerHandCategory.is_royal_straight_flush(hand): return PokerHandCategory.ROYAL_STRAIGHT_FLUSH
        elif PokerHandCategory.is_straight_flush(hand): return PokerHandCategory.STRAIGHT_FLUSH
        elif PokerHandCategory.is_four_of_a_kind(hand): return PokerHandCategory.FOUR_OF_A_KIND
        elif PokerHandCategory.is_full_house(hand): return PokerHandCategory.FULL_HOUSE
        elif PokerHandCategory.is_flush(hand): return PokerHandCategory.FLUSH
        elif PokerHandCategory.is_straight(hand): return PokerHandCategory.STRAIGHT
        elif PokerHandCategory.is_three_of_a_kind(hand): return PokerHandCategory.THREE_OF_A_KIND
        elif PokerHandCategory.is_two_pair(hand): return PokerHandCategory.TWO_PAIR
        elif PokerHandCategory.is_one_pair(hand): return PokerHandCategory.ONE_PAIR
        else: return PokerHandCategory.HIGH_CARD

    @staticmethod
    def is_royal_straight_flush(hand: Cards) -> bool:
        return isinstance(hand, Cards) and PokerHandCategory._is_straight(hand, royal=True) and PokerHandCategory._is_flush(hand)

    @staticmethod
    def is_straight_flush(hand: Cards) -> bool:
        return isinstance(hand, Cards) and PokerHandCategory._is_straight(hand) and PokerHandCategory._is_flush(hand)

    @staticmethod
    def is_four_of_a_kind(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            if 4 in Counter(card.rank for card in hand).values():
                return True
        return False

    @staticmethod
    def is_full_house(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            rank_counts = {}
            for card in hand:
                rank_counts[card.rank] = rank_counts.get(card.rank, 0) + 1
            if len(rank_counts) == 2:
                return sorted(rank_counts.values()) == [2, 3]
        return False

    @staticmethod
    def is_flush(hand: Cards) -> bool:
        return isinstance(hand, Cards) and PokerHandCategory._is_flush(hand) and (not PokerHandCategory.is_straight_flush(hand))

    @staticmethod
    def is_straight(hand: Cards) -> bool:
        return isinstance(hand, Cards) and PokerHandCategory._is_straight(hand) and (not PokerHandCategory.is_straight_flush(hand))

    @staticmethod
    def is_three_of_a_kind(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            if 3 in Counter(card.rank for card in hand).values():
                if not PokerHandCategory.is_full_house(hand):
                    return True
        return False

    @staticmethod
    def is_two_pair(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            rank_counts = {}
            for card in hand:
                rank_counts[card.rank] = rank_counts.get(card.rank, 0) + 1
            if len(rank_counts) == 3:
                return sorted(rank_counts.values()) == [1, 2, 2]
        return False

    @staticmethod
    def is_one_pair(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            rank_counts = {}
            for card in hand:
                rank_counts[card.rank] = rank_counts.get(card.rank, 0) + 1
            if len(rank_counts) == 4:
                return sorted(rank_counts.values()) == [1, 1, 1, 2]
        return False

    @staticmethod
    def _is_flush(hand: Cards) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            if len({card.suit for card in hand}) == 1:
                return True
        return False

    @staticmethod
    def _is_straight(hand: Cards, royal: bool = False) -> bool:
        if isinstance(hand, Cards) and (hand.count == PokerHand.NCARDS):
            ranks = sorted(card.rank.value for card in hand)
            if len(set(ranks)) == PokerHand.NCARDS:
                if ranks == [Rank.ACE.value, Rank.TEN.value, Rank.JACK.value, Rank.QUEEN.value, Rank.KING.value]:
                    return True 
                if (royal is not True) and ((ranks[-1] - ranks[0]) == 4):
                    return True
        return False

    def __str__(self) -> str:
        match self:
            case PokerHandCategory.HIGH_CARD: return "high card"
            case PokerHandCategory.ONE_PAIR: return "one pair"
            case PokerHandCategory.TWO_PAIR: return "two pair"
            case PokerHandCategory.THREE_OF_A_KIND: return "three of a kind"
            case PokerHandCategory.STRAIGHT: return "straight"
            case PokerHandCategory.FLUSH: return "flush"
            case PokerHandCategory.FULL_HOUSE: return "full house"
            case PokerHandCategory.FOUR_OF_A_KIND: return "four of a kind"
            case PokerHandCategory.STRAIGHT_FLUSH: return "straight flush"
            case PokerHandCategory.ROYAL_STRAIGHT_FLUSH: return "royal straight flush"
            case _: return ""


class PokerHandStats:

    def __init__(self) -> None:
        self._hand_total = 0
        self._hand_ranks = {
            PokerHandCategory.HIGH_CARD: 0,
            PokerHandCategory.ONE_PAIR: 0,
            PokerHandCategory.TWO_PAIR: 0,
            PokerHandCategory.THREE_OF_A_KIND: 0,
            PokerHandCategory.STRAIGHT: 0,
            PokerHandCategory.FLUSH: 0,
            PokerHandCategory.FULL_HOUSE: 0,
            PokerHandCategory.FOUR_OF_A_KIND: 0,
            PokerHandCategory.STRAIGHT_FLUSH: 0,
            PokerHandCategory.ROYAL_STRAIGHT_FLUSH: 0
        }

    def note_hand(self, hand: PokerHand) -> None:
        if isinstance(hand, PokerHand):
            self._hand_ranks[hand.evaluate_category()] += 1
            self._hand_total += 1

    def print_hand_stats(self, verbose: bool = False) -> None:
        if verbose is True:
            print(f"{'total':<21} {Chars.DOT}{Chars.RARROW} {self._hand_total:>6} {Chars.DOT} {100:>8.4f}%")
        t = 0
        for hand_rank in self._hand_ranks:
            count = self._hand_ranks[hand_rank]
            t += count
            percent = (count / self._hand_total) * 100.0
            print(f"{hand_rank:<21} {Chars.DOT}{Chars.RARROW} {count:>6} {Chars.DOT} {percent:>8.4f}%")


if False:
    deck = Deck()
    nhands = 1000
    hand_stats = PokerHandStats()
    for n in range(nhands):
        deck = Deck()
        hand = PokerHand.create(deck)
        hand_stats.note_hand(hand)
        # print(f"{hand} {Chars.RARROW} {hand.evaluate_category()}")
    hand_stats.print_hand_stats()

if False:
    original_deck = Deck()
    original_hand = PokerHand(original_deck.take_random(), original_deck.take_random())
    print(f"Given: {str(original_hand).strip()} ...")
    nhands = 1000
    hand_stats = PokerHandStats()
    for n in range(nhands):
        deck = original_deck.copy()
        hand = original_hand.copy()
        hand.fill(deck)
        hand_stats.note_hand(hand)
        # print(f"{hand} {Chars.RARROW} {hand.evaluate_category()}")
    hand_stats.print_hand_stats()

if False:
    #
    # Actual full results ...
    #
    # total                 ·➤ 2598960 · 100.0000%
    # high card             ·➤ 1302540 ·  50.1177%
    # one pair              ·➤ 1098240 ·  42.2569%
    # two pair              ·➤  123552 ·   4.7539%
    # three of a kind       ·➤   54912 ·   2.1128%
    # straight              ·➤   10200 ·   0.3925%
    # flush                 ·➤    5108 ·   0.1965%
    # full house            ·➤    3744 ·   0.1441%
    # four of a kind        ·➤     624 ·   0.0240%
    # straight flush        ·➤      36 ·   0.0014%
    # royal straight flush  ·➤       4 ·   0.0002%
    #
    hands = []
    hand_stats = PokerHandStats()
    def f(hand: Cards) -> None:
        global hand_stats, hands
        hand_stats.note_hand(hand)
        hands.append(hand)
        hand_category, hand_category_ranks = hand.evaluate()
        hand_rating_name = PokerHandCategory(hand_category)
        hand_category_ranks = " ".join(map(str, map(Rank.create, hand_category_ranks)))
        print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {hand_rating_name} {Chars.DOT} {hand_category_ranks}")
        #print(f"{hand_stats._hand_total}\r", end="")
        #print(f"{hand_stats._hand_total}\r", end="")
    PokerHand.visit_possible_hands(f)
    hand_stats.print_hand_stats()
    ranked_hands = PokerHandCategory.evaluate_hands(hands)
    print("RANKED HANDS")
    for hand in ranked_hands:
        hand_category, hand_category_ranks = hand.evaluate()
        hand_rating_name = PokerHandCategory(hand_category)
        hand_category_ranks = " ".join(map(str, map(Rank.create, hand_category_ranks)))
        print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {hand_rating_name} {Chars.DOT} {hand_category_ranks}")

if False:
    card = Card.create("ah")
    print(card)

if False:
    cards = Cards(
        Card(Rank.ACE, Suit.HEARTS),
        Card(Rank.TWO, Suit.HEARTS),
        Card(Rank.THREE, Suit.HEARTS),
        Card(Rank.FOUR, Suit.HEARTS),
        Card(Rank.FIVE, Suit.HEARTS),
        Card(Rank.SIX, Suit.HEARTS),
        Card(Rank.SEVEN, Suit.HEARTS)
    )
    hands = PokerHand.get_possible_hands(cards)
    for hand in hands:
        print(hand)
        print(hand.has_straight_flush())

if False:
    original_deck = Deck()

    public_cards = Cards()
    for n in range(5):
        public_cards.add(original_deck.take_top())

    player_hand = PokerHand.create(original_deck, 2)

    print(f"Public cards: {public_cards}")
    print(f"My cards:     {player_hand}")
    player_possible_hands = PokerHandCategory.evaluate_hands(PokerHand.get_possible_hands(player_hand + public_cards))
    player_best_hand = player_possible_hands[0]
    player_best_hand_category = player_best_hand.evaluate_category()
    print(f"My best hand: {player_best_hand.to_string()}")
    print(f"My possible hands: {len(player_possible_hands)}")
    for player_possible_hand in player_possible_hands:
        print(player_possible_hand.to_string())


if False:
    hands = []
    hands.append(PokerHand(
        Card(Rank.FOUR, Suit.HEARTS),
        Card(Rank.THREE, Suit.DIAMONDS),
        Card(Rank.SIX, Suit.CLUBS),
        Card(Rank.NINE, Suit.SPADES),
        Card(Rank.FIVE, Suit.CLUBS),
        Card(Rank.SIX, Suit.HEARTS),
        Card(Rank.SEVEN, Suit.HEARTS)
    ))
    hands.append(PokerHand(
        Card(Rank.KING, Suit.SPADES),
        Card(Rank.TWO, Suit.HEARTS),
        Card(Rank.THREE, Suit.HEARTS),
        Card(Rank.THREE, Suit.CLUBS),
        Card(Rank.FIVE, Suit.SPADES),
        Card(Rank.SIX, Suit.HEARTS),
        Card(Rank.SEVEN, Suit.DIAMONDS)
    ))
    print()
    #x = evaluate_hand_with_rank(hands[0])
    #y = evaluate_hand_with_rank(hands[1])
    #x = hands[0].evaluate()
    #y = hands[1].evaluate()
    #print(hands[0])
    #print(x)
    #print(hands[1])
    #print(y)

if False:
    deck = Deck()
    hands = []
    for n in range(100):
        hand = PokerHand.create(deck.copy())
        hands.append(hand)
    ranked_hands = PokerHandCategory.evaluate_hands(hands)
    #x = PokerHandCategory.evaluate_hands(hands)
    for hand in ranked_hands:
        hand_category, hand_category_ranks = hand.evaluate()
        hand_rating_name = PokerHandCategory(hand_category)
        hand_category_ranks = " ".join(map(str, map(Rank.create, hand_category_ranks)))
        print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {hand_rating_name} {Chars.DOT} {hand_category_ranks}")
        #print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {Chars.RARROW} {PokerHandCategory(hand_rating_name)} {Chars.DOT} {' '.join(map(str, hand_category_ranks))}")
        #print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {hand.evaluate_category()} {Chars.RARROW} {hand.evaluate()}")
        #print(f"{hand.sorted(reverse=False)}  {Chars.DOT}{Chars.RARROW} {hand.evaluate_category()} {Chars.RARROW} {PokerHandCategory(hand.evaluate()[0])}")

if False:
    x = PokerHand.create("  4 ♦   10 ♦   Q ♦  10 ♥   8 ♠")
    #x = PokerHand.create("2 ♦   7 ♦   2 ♥   Q ♥   K ♥")
    print('xxx')
    print(x)
    print(x.copy())
    print(x.has_one_pair())

if False:
    line = input()
    hand = PokerHand(Cards.create(line, nodups=True))
    deck = Deck()
    for card in hand:
        deck.remove(card)
    hand.fill(deck)
    print(hand)

if False:
    deck = Deck()
    #import pdb ; pdb.set_trace()
    hand_stats = PokerHandStats()
    hand = PokerHand(deck.take_random(2))
    def visit(cards):
        global hand, hand_stats
        hand_stats.note_hand(hand + cards)
    PokerHand.visit_possible_hands(ncards=3, deck=deck, callback=visit)
    print(f"hand:                 {Chars.DOT}{Chars.RARROW} " + str(hand))
    hand_stats.print_hand_stats()

#import pdb ; pdb.set_trace()
#cards = Cards.create("AH 10s")
#deck = Deck()
##import pdb ; pdb.set_trace()
#print(cards)
#x = deck.remove(123)
#print(x)
#print(deck.count)
