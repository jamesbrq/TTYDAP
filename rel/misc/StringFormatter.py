def create_string_table(filename):
  with open(filename, 'wb') as f:
    for key, item in strings_dict.items():
        f.write(key.encode('utf-8'))
        f.write(b'\x00')  # null terminator
        f.write(item.encode('utf-8'))
        f.write(b'\x00')  # null terminator
    f.write(b'\x00')  # null terminator for the end of the table

strings_dict = {
    "jolene_fukidashi": "Oh my!<wait 100> Excuse me.\n<k>",

    "jolene_fukidashi_end": "Well then.<wait 100> Shall we\nget going?\n<k>",

    "grubba_bribe": ("Well, well, well! If it ain't\n"
                     "my star fighter! Hoo-WEE!\n<k>\n<p>\n"
                     "Listen here... I been watchin'\n"
                     "you climb them ranks, and let\n"
                     "me tell ya somethin'...\n<k>\n<p>\n"
                     "You got potential, son! Real\n"
                     "championship material! But\n"
                     "this ranking business...\n<k>\n<p>\n"
                     "Well, it can be slower than\n"
                     "molasses sometimes, if you\n"
                     "catch my drift...\n<k>\n<p>\n"
                     "Now, between you and me,\n"
                     "old Grubba's got some...\n"
                     "pull around these parts.\n<k>\n<p>\n"
                     "What do ya say we make a little\n"
                     "gentleman's agreement?\n<k>\n<p>\n"
                     "How many ranks you want to move?\n"
                     "Up or down, doesn't matter!\n"
                     "Just pick a number!\n<o>"),

    "grubba_rank_same": ("<p>\nHold on there, son! You're\n"
                         "already at that rank!\n<k>\n<p>\n"
                         "What are ya tryin' to pull\n"
                         "here? Heh heh...\n<k>\n<p>\n"
                         "Come back when you actually\n"
                         "want to move somewhere!\n<k>"),

    "grubba_pay": ("<p>\nSo you're talkin' about movin'\n"
                   "that many ranks, eh?\n<k>\n<p>\n"
                   "That'll cost ya about <NUM> coin<S>.\n"
                   "No problem for old Grubba!\n<k>\n<p>\n"
                   "You'd be movin' faster than\n"
                   "a Buzzy Beetle up a pipe!\n<k>\n<p>\n"
                   "Ain't nobody gotta know about\n"
                   "our little arrangement, you\n"
                   "read me here?\n<o>"),

    "grubba_pay_prompt": ("<select 0 2 0 60>\nYou got yourself a deal!\n"
                          "That ain't right, Grubba."),

    "grubba_no_coins": ("<p>\nWell, I'll be! Looks like\n"
                        "you're a little light in the\n"
                        "pockets there, son!\n<k>\n<p>\n"
                        "Heh heh... Can't squeeze blood\n"
                        "from a turnip, as they say!\n<k>\n<p>\n"
                        "Tell ya what... Go earn yourself\n"
                        "some more coin, then come back\n"
                        "and see old Grubba!\n<k>\n<p>\n"
                        "A deal this good ain't gonna\n"
                        "last forever, you hear?\n<k>"),

    "grubba_pay_accept": ("<p>\nHoo-WEE! Now that's what I like\n"
                          "to hear, son!\n<k>\n<p>\n"
                          "Just hand over them coins and...\n"
                          "BAM! You'll be movin' ranks\n"
                          "faster than you can say...\n<k>\n<p>\n"
                          "\"Great Gonzales\"!\n"
                          "Ain't that just DYNAMITE?\n<k>\n<p>\n"
                          "Pleasure doin' business with ya!\n"
                          "Now get back in there and\n"
                          "show 'em what for!\n<k>"),

    "grubba_pay_accept_no": ("<p>\nWell, I'll be... Heh heh...\n"
                             "You got some real integrity\n"
                             "there, son!\n<k>\n<p>\n"
                             "Can't say I don't respect a\n"
                             "fighter with principles...\n"
                             "Even if it ain't good business!\n<k>\n<p>\n"
                             "Tell ya what... The offer's always\n"
                             "on the table if you change your\n"
                             "mind, you hear?\n<k>\n<p>\n"
                             "Now get back in there and show\n"
                             "'em what the Great Gonzales\n"
                             "is made of! Hoo-WEE!\n<k>"),

    "grubba_pay_reject": ("<p>\nAw, come on now! Don't be\n"
                          "such a penny-pincher!\n<k>\n<p>\n"
                          "A fighter's gotta invest in\n"
                          "his future if he wants to\n"
                          "make it to the big time!\n<k>\n<p>\n"
                          "But I get it, son... Times are\n"
                          "tough all over. Come back when\n"
                          "you got some coin to spare!\n<k>"),

    "pit_checkpoint_explain": ("<system>\nEvery 10 floors cleared\n"
                               "in the pit will unlock a\n"
                               "checkpoint.\n<k>\n<p>\n"
                               "Selecting a checkpoint will\n"
                               "send you to that floor the\n"
                               "next time you take the pipe.\n<k>"),

    "pit_checkpoint": ("<system>\nSelect a floor you would like\n"
                       "to start from the next\n"
                       "time you enter the pit.\n<o>"),

    "pit_checkpoint_deny": ("<system>\nYou have not unlocked\n"
                            "any pit checkpoints.\n<k>"),

    "pit_checkpoint_set": "<p>\nYour pit checkpoint\nhas been set.\n<k>",

    "madam_abort": "<p>Oh silly me, I already have\nmy ring.\n<k>",

    "keelhaul_return": "Would you like to go back to\nKeelhaul Key?\n<o>",

    "keelhaul_return_yn": "<select 0 0 0 40>Yes\nNo",

    "keelhaul_return_yes": "<p>Let's get going then.<k>",

    "keelhaul_return_no": "<p>I'll be here if you ever\nneed to get back.\n<k>",

    "goldbob_guide": ("I'm going to need to see\n"
                      "the Goldbob Guide if you are going\n"
                      "to use the cannon.\n<k>"),

    "no_goldbob_guide": "Sorry pal. No guide,\nno cannon.\n<k>",

    "give_goldbob_guide": "Thanks, let's begin then.\n<k>",

    "no_diary": "<p><col 000000a0><shake>I need my diary.\n</shake></col>\n<k>",

    "no_paper": "<p>Gonna need that paper.\n<k>",

    "no_blanket": "<p>I really need that blanket.\n<k>",

    "raise_text": ("<system>Would you like to raise the\n"
                   "floor level of the hallway?\n<o>"),

    "raise_text2": "<system><p>The floor was raised.\n<k>",

    "raise_text_yn": "<select 0 1 0 40>Yes\nNo",

    "lower_text": ("<system>Would you like to lower the\n"
                   "floor level of the hallway?\n<o>"),

    "lower_text2": "<system><p>The floor was lowered.\n<k>",

    "lower_text_yn": "<select 0 1 0 40>Yes\nNo",

    "ap_item": "AP Item",

    "ap_item_desc": "AP Item",

    "return_pipe": "Return Pipe",

    "return_pipe_desc": "A magic pipe that returns you\nto Rogueport from afar.",

    "10_coins": "10 Coins",

    "10_coins_desc": "A handful of coins.",

    "plane_mode": "Plane Mode",

    "plane_mode_desc": "Plane Mode",

    "paper_mode": "Paper Mode",

    "paper_mode_desc": "Paper Mode",

    "tube_mode": "Tube Mode",

    "tube_mode_desc": "Tube Mode",

    "boat_mode": "Boat Mode",

    "boat_mode_desc": "Boat Mode",

    # Key Renames
    "elevator_key_station": "Elevator Key (Station)",

    "card_key_1": "Card Key 1",

    "card_key_2": "Card Key 2",

    "card_key_3": "Card Key 3",

    "card_key_4": "Card Key 4",

    "palace_key_tower": "Palace Key (Tower)",

    "elevator_key_1": "Elevator Key 1",

    "elevator_key_2": "Elevator Key 2",

    "station_key_1": "Station Key 1",

    "station_key_2": "Station Key 2",

    "storage_key_1": "Storage Key 1",

    "storage_key_2": "Storage Key 2",

    "black_key_plane": "Black Key (Plane)",

    "black_key_paper": "Black Key (Paper)",

    "black_key_tube": "Black Key (Tube)",

    "black_key_boat": "Black Key (Boat)",

    # Progressive Renames
    "progressive_boots": "Progressive Boots",

    "progressive_hammer": "Progressive Hammer",

    # Map Nodes
    "menu_monosiri_rsh": "Excess Express",

    "menu_monosiri_las_09": "Riddle Tower",

    "menu_map_rsh": ("The ultimate in locomotive\n"
                     "travel, famed not only for\n"
                     "its stunning appearance, but\n"
                     "its comfortable rides as well."),

    "menu_map_las_09": ("A mysterious tower located\n"
                        "inside the Palace of Shadow.\n"
                        "Each of its many rooms\n"
                        "contains a riddle."),
}