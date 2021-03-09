//
//  CardView.swift
//  SetGame2
//
//  Created by David Michaels on 3/1/21.
//

import SwiftUI

struct CardView : View {
    
    @ObservedObject var card : TableCard;
    var touchedCallback : (TableCard) -> Void = dummyTouchedCallback;
    static func dummyTouchedCallback(_ card : TableCard) {
        //card.selected.toggle();
    }
    var body : some View {
        VStack {
            Button(action: {touchedCallback(card)}) {
            Image(card.codename)
                .resizable()
                .aspectRatio(contentMode: .fit)
                .rotation3DEffect(card.selected ? Angle(degrees: 720) : (card.selected ? Angle(degrees: 360) : Angle(degrees: 0)), axis: (x: CGFloat(0), y: CGFloat(card.selected ? 0 : 1), z: CGFloat(card.selected ? 1 : 0)))
            .animation(Animation.linear(duration: 0.3))
            .cornerRadius(8)
                .overlay(RoundedRectangle(cornerRadius:card.selected ? 10 : 6)
                            .stroke(card.selected ? Color.red : Color.gray, lineWidth: card.selected ? 3 : 1))
                .shadow(color: card.selected ? Color.green : Color.blue, radius: card.selected ? 3 : 1)
                .padding(1)
                .onTapGesture {
                    touchedCallback(card);
                }
            }
        }
    }
}

struct CardView_Previews: PreviewProvider {
    static func touchedCallback(_ card : TableCard) {
        card.selected.toggle();
    }
    static var previews: some View {
        let _ : TableCard = TableCard("PQT3")!;
        CardView(card: TableCard(), touchedCallback: touchedCallback);
    }
}
