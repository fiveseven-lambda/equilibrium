# equilibrium
simulation of equilibrium of electrolytic dissociation.

When you conduct an acid-base titration experiment in the laboratory, you put one solution in a flask and drip the other solution from a burette. If you want to get weaker solution, you can dilute it. If you made too much solution, you can throw it away.
In this program, you can do everything you can do in a laboratory.

HOW TO USE

The following are always shown in the program:
 amount of the solution in the flask
 concentration of each substance
 pH of the solution
 pKw and pKa of each equlibrium (to assure the quality of the simulation)

Press key a, b, ... to drip solution which contains H+, OH-, ... .

Press return to enter command.
The list of command:
 "amount" - you can change the amount of solution in the flask (unit: L, default: 100 mL).
 "discard" - you can discard all of the solution (equal to "amount" "0").
 "conc" - you can change the concentration of dropping solution (unit: mol/L, default: .01).
 "water" - you can use water to dilute solution (equal to "conc" "0").
 "add" - you can change the installment of solution you add (unit: L, default: 5e-5).
 "drop" - you can drip solution drop by drop (equal to "add" "5e-5").
 "quit" - quit.
 
TUTORIAL

When you start the program, the flask contains 100 mL of water.
Press 'a' several times to add some acid to the water, or 'b' to add some base. Every time you press the key, 5e-5 liter of .01 mol/L acid/base will be added. This acid/base is totally ionized (pKa is negative infinity). In fact, hydrochloric acid and sodium hydroxide is almost totally ionized in water.
Then press return, enter "discard" and press return again to discard all.
Press return, enter "conc" and press return. Then enter ".1" and press return again. You now have .1 mol/L acetic acid and .1 mol/L sodium acetate solution.
Press return, enter "add", press return, enter ".1" and press return. Then press 'c' once to add 100 mL acetic acid, and press 'd' once to add 100 mL sodium acetate solution. This solution is called buffer, and known to keep its pH when added acid or base, or deluted.
Press return, enter "conc", press return, enter ".01" and press return. Then press return, enter "drop" and press return. Try adding some acid or base, and you can see pH hardly changes.
Press return, enter "amount", press return, enter ".01" and press return. Now 10 mL of solution is left in the flask. Then delute - press return, enter "water" and press return. Press return, enter "add", press return enter ".09" and press return. Press key 'a' or 'b' or 'c' or 'd'(actually any dropping solution is now pure water, so any will do). 90 mL of water is added, but pH doesn't change.

