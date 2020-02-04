#include "Localization.h"
#include "..\..\Model\Config\Config.h"
#include <stdlib.h>
#include <stdio.h>

#define NBR_TEXT 188

LocalizationArray MainLocalization = { 0 };

LocalizationArray* GetPointerMainLocalization()
{
	return &MainLocalization;
}

LocalizationArray GetMainLocalization()
{
	return MainLocalization;
}

void InitializeLocalization(LocalizationArray* pArrayString, const int pLanguage)
{
	pArrayString->Array = (Localization*)malloc(sizeof(Localization) * NBR_TEXT);
	pArrayString->Size = 0;
	switch (pLanguage)
	{
	case FR:
		AddLocalizationString(pArrayString, "Jouer");//0
		AddLocalizationString(pArrayString, "Charger");//1
		AddLocalizationString(pArrayString, "Options");//2
		AddLocalizationString(pArrayString, "Quitter");//3
		AddLocalizationString(pArrayString, "Un Joueur");//4
		AddLocalizationString(pArrayString, "Deux Joueurs");//5
		AddLocalizationString(pArrayString, "Retour");//6
		AddLocalizationString(pArrayString, "Fenêtre");//7
		AddLocalizationString(pArrayString, "Plein Ecran");//8
		AddLocalizationString(pArrayString, "Sans Bordures");//9
		AddLocalizationString(pArrayString, "Resolution");//10
		AddLocalizationString(pArrayString, "Affichage");//11
		AddLocalizationString(pArrayString, "Boule de Feu");//12
		AddLocalizationString(pArrayString, "Cone de Feu");//13
		AddLocalizationString(pArrayString, "Reprendre");//14
		AddLocalizationString(pArrayString, "Retourner au Menu");//15
		AddLocalizationString(pArrayString, "Langage");//16
		AddLocalizationString(pArrayString, "Français");//17
		AddLocalizationString(pArrayString, "English");//18
		AddLocalizationString(pArrayString, "Piège de feu");//19
		AddLocalizationString(pArrayString, "Général");//20
		AddLocalizationString(pArrayString, "Graphismes");//21
		AddLocalizationString(pArrayString, "Sons");//22
		AddLocalizationString(pArrayString, "Contrôles");//23
		AddLocalizationString(pArrayString, "Haut");//24
		AddLocalizationString(pArrayString, "Bas");//25
		AddLocalizationString(pArrayString, "Gauche");//26
		AddLocalizationString(pArrayString, "Droite");//27
		AddLocalizationString(pArrayString, "Boule De Feu");//28
		AddLocalizationString(pArrayString, "Cone De Feu");//29
		AddLocalizationString(pArrayString, "Piège De Feu");//30
		AddLocalizationString(pArrayString, "Roulade");//31
		AddLocalizationString(pArrayString, "Carte");//32
		AddLocalizationString(pArrayString, "Attaque");//33
		AddLocalizationString(pArrayString, "Utiliser Sort");//34


		AddLocalizationString(pArrayString, "");//35
		AddLocalizationString(pArrayString, "A");//36
		AddLocalizationString(pArrayString, "B");//37
		AddLocalizationString(pArrayString, "C");//38
		AddLocalizationString(pArrayString, "D");//39
		AddLocalizationString(pArrayString, "E");//40
		AddLocalizationString(pArrayString, "F");//41
		AddLocalizationString(pArrayString, "G");//42
		AddLocalizationString(pArrayString, "H");//43
		AddLocalizationString(pArrayString, "I");//44
		AddLocalizationString(pArrayString, "J");//45
		AddLocalizationString(pArrayString, "K");//46
		AddLocalizationString(pArrayString, "L");//47
		AddLocalizationString(pArrayString, "M");//48
		AddLocalizationString(pArrayString, "N");//49
		AddLocalizationString(pArrayString, "O");//50
		AddLocalizationString(pArrayString, "P");//51
		AddLocalizationString(pArrayString, "Q");//52
		AddLocalizationString(pArrayString, "R");//53
		AddLocalizationString(pArrayString, "S");//54
		AddLocalizationString(pArrayString, "T");//55
		AddLocalizationString(pArrayString, "U");//56
		AddLocalizationString(pArrayString, "V");//57
		AddLocalizationString(pArrayString, "W");//58
		AddLocalizationString(pArrayString, "X");//59
		AddLocalizationString(pArrayString, "Y");//60
		AddLocalizationString(pArrayString, "Z");//61
		AddLocalizationString(pArrayString, "à");//62
		AddLocalizationString(pArrayString, "&");//63
		AddLocalizationString(pArrayString, "é");//64
		AddLocalizationString(pArrayString, "\"");//65
		AddLocalizationString(pArrayString, "\'");//66
		AddLocalizationString(pArrayString, "(");//67
		AddLocalizationString(pArrayString, "-");//68
		AddLocalizationString(pArrayString, "è");//69
		AddLocalizationString(pArrayString, "_");//70
		AddLocalizationString(pArrayString, "ç");//71
		AddLocalizationString(pArrayString, "Echap");//72
		AddLocalizationString(pArrayString, "LControl");//73
		AddLocalizationString(pArrayString, "LShift");//74
		AddLocalizationString(pArrayString, "LAlt");//75
		AddLocalizationString(pArrayString, "LSystem");//76
		AddLocalizationString(pArrayString, "RControl");//77
		AddLocalizationString(pArrayString, "RShift");//78
		AddLocalizationString(pArrayString, "RAlt");//79
		AddLocalizationString(pArrayString, "RSystem");//80
		AddLocalizationString(pArrayString, "Menu");//81
		AddLocalizationString(pArrayString, "[");//82
		AddLocalizationString(pArrayString, "]");//83
		AddLocalizationString(pArrayString, ";");//84
		AddLocalizationString(pArrayString, ",");//85
		AddLocalizationString(pArrayString, ".");//86
		AddLocalizationString(pArrayString, "'");//87
		AddLocalizationString(pArrayString, "/");//88
		AddLocalizationString(pArrayString, "\\");//89
		AddLocalizationString(pArrayString, "~");//90
		AddLocalizationString(pArrayString, "=");//91
		AddLocalizationString(pArrayString, "-");//92
		AddLocalizationString(pArrayString, "Espace");//93
		AddLocalizationString(pArrayString, "Entrée");//94
		AddLocalizationString(pArrayString, "Retour");//95
		AddLocalizationString(pArrayString, "Tab");//96
		AddLocalizationString(pArrayString, "Page Haut");//97
		AddLocalizationString(pArrayString, "Page Bas");//98
		AddLocalizationString(pArrayString, "Fin");//99
		AddLocalizationString(pArrayString, "Home");//100
		AddLocalizationString(pArrayString, "Inser");//101
		AddLocalizationString(pArrayString, "Suppr");//102
		AddLocalizationString(pArrayString, "+");//103
		AddLocalizationString(pArrayString, "-");//104
		AddLocalizationString(pArrayString, "*");//105
		AddLocalizationString(pArrayString, "/");//106
		AddLocalizationString(pArrayString, "Gauche");//107
		AddLocalizationString(pArrayString, "Droite");//108
		AddLocalizationString(pArrayString, "Haut");//109
		AddLocalizationString(pArrayString, "Bas");//110
		AddLocalizationString(pArrayString, "NumPad0");//111
		AddLocalizationString(pArrayString, "NumPad1");//112
		AddLocalizationString(pArrayString, "NumPad2");//113
		AddLocalizationString(pArrayString, "NumPad3");//114
		AddLocalizationString(pArrayString, "NumPad4");//115
		AddLocalizationString(pArrayString, "NumPad5");//116
		AddLocalizationString(pArrayString, "NumPad6");//117
		AddLocalizationString(pArrayString, "NumPad7");//118
		AddLocalizationString(pArrayString, "NumPad8");//119
		AddLocalizationString(pArrayString, "NumPad9");//120
		AddLocalizationString(pArrayString, "F1");//121
		AddLocalizationString(pArrayString, "F2");//122
		AddLocalizationString(pArrayString, "F3");//123
		AddLocalizationString(pArrayString, "F4");//124
		AddLocalizationString(pArrayString, "F5");//125
		AddLocalizationString(pArrayString, "F6");//126
		AddLocalizationString(pArrayString, "F7");//127
		AddLocalizationString(pArrayString, "F8");//128
		AddLocalizationString(pArrayString, "F9");//129
		AddLocalizationString(pArrayString, "F10");//130
		AddLocalizationString(pArrayString, "F11");//131
		AddLocalizationString(pArrayString, "F12");//132
		AddLocalizationString(pArrayString, "F13");//133
		AddLocalizationString(pArrayString, "F14");//134
		AddLocalizationString(pArrayString, "F15");//135
		AddLocalizationString(pArrayString, "Pause");//136

		AddLocalizationString(pArrayString, "");//137
		AddLocalizationString(pArrayString, "Clic Gauche");//138
		AddLocalizationString(pArrayString, "Clic Droit");//139
		AddLocalizationString(pArrayString, "Clic Molette");//140
		AddLocalizationString(pArrayString, "Bouton Souris 1");//141
		AddLocalizationString(pArrayString, "Bouton Souris 2");//142

		AddLocalizationString(pArrayString, "Lumière Dynamique"); //143

		AddLocalizationString(pArrayString, "Sauvegarde"); //144

		AddLocalizationString(pArrayString, "Vide"); //145

		AddLocalizationString(pArrayString, "Joueur un"); //146
		AddLocalizationString(pArrayString, "Joueur deux"); //147

		AddLocalizationString(pArrayString, "Clavier"); //148
		AddLocalizationString(pArrayString, "Manette"); //149
		AddLocalizationString(pArrayString, "Aucun"); //150

		AddLocalizationString(pArrayString, "Raccourcis"); //151

		AddLocalizationString(pArrayString, "Désactivé(e)"); //152
		AddLocalizationString(pArrayString, "Activé(e)"); //153

		AddLocalizationString(pArrayString, "Général"); // 154
		AddLocalizationString(pArrayString, "Musique"); // 155
		AddLocalizationString(pArrayString, "Effets Sonores"); // 156

		AddLocalizationString(pArrayString, "Nouvelle"); // 157
		AddLocalizationString(pArrayString, "Crédits"); // 158

		AddLocalizationString(pArrayString, "Les potions de vie\nrendent plus de vie."); // 159
		AddLocalizationString(pArrayString, "Le temps\nd'invulnérabilité\nest augmenté."); // 160
		AddLocalizationString(pArrayString, "La vie maximum\nest augmentée."); // 161

		AddLocalizationString(pArrayString, "Les potions de mana\nrendent plus de\nmana."); // 162
		AddLocalizationString(pArrayString, "Les coûts en mana\nsont réduits."); // 163
		AddLocalizationString(pArrayString, "Le mana maximum\nest augmenté."); // 164

		AddLocalizationString(pArrayString, "Le piège de feu tire\ntrois boules de feu\nlors de l'explosion."); // 165
		AddLocalizationString(pArrayString, "Tire trois boules\nde feu au lieu d'une."); // 166
		AddLocalizationString(pArrayString, "Augmente les dégats\ndes attaques."); // 167

		AddLocalizationString(pArrayString, "La vitesse maximale\nest augmentée."); // 168
		AddLocalizationString(pArrayString, "Les effets\nincapacitant le\njoueur sont réduits."); // 169
		AddLocalizationString(pArrayString, "Le Dash possède\nune seconde charge."); // 170

		AddLocalizationString(pArrayString, "Pas encore débloqué."); // 171
		AddLocalizationString(pArrayString, "Amélioré au\nmaximum."); // 172
		AddLocalizationString(pArrayString, "Coût"); // 173
		AddLocalizationString(pArrayString, "Votre XP"); // 174

		AddLocalizationString(pArrayString, "Un Joueur"); //175
		AddLocalizationString(pArrayString, "Deux Joueurs"); //176

		AddLocalizationString(pArrayString, "Slime"); //177
		AddLocalizationString(pArrayString, "Elementaire"); //178

		AddLocalizationString(pArrayString, "Mode"); //179
		AddLocalizationString(pArrayString, "Donjon"); //180
		AddLocalizationString(pArrayString, "Nombre de joueur"); //181

		AddLocalizationString(pArrayString, "Campagne"); //182
		AddLocalizationString(pArrayString, "Personnalisé"); //183

		AddLocalizationString(pArrayString, "Donjon\nSlime");//184
		AddLocalizationString(pArrayString, "Donjon\nElementaire");//185

		AddLocalizationString(pArrayString, "Introduction");//186
		AddLocalizationString(pArrayString, "Razar");//187
		break;
	case EN:
		AddLocalizationString(pArrayString, "Play");//0
		AddLocalizationString(pArrayString, "Load");//1
		AddLocalizationString(pArrayString, "Options");//2
		AddLocalizationString(pArrayString, "Quit");//3
		AddLocalizationString(pArrayString, "One player");//4
		AddLocalizationString(pArrayString, "Two players");//5
		AddLocalizationString(pArrayString, "Back");//6
		AddLocalizationString(pArrayString, "Windowed");//7
		AddLocalizationString(pArrayString, "FullScreen");//8
		AddLocalizationString(pArrayString, "Borderless");//9
		AddLocalizationString(pArrayString, "Resolution");//10
		AddLocalizationString(pArrayString, "Window Mode");//11
		AddLocalizationString(pArrayString, "Fire ball");//12
		AddLocalizationString(pArrayString, "Cone of fire");//13
		AddLocalizationString(pArrayString, "Continue");//14
		AddLocalizationString(pArrayString, "Return to Menu");//15
		AddLocalizationString(pArrayString, "Language");//16
		AddLocalizationString(pArrayString, "Français");//17
		AddLocalizationString(pArrayString, "English");//18
		AddLocalizationString(pArrayString, "Fire Trap");//19
		AddLocalizationString(pArrayString, "General");//20
		AddLocalizationString(pArrayString, "Graphics");//21
		AddLocalizationString(pArrayString, "Sounds");//22
		AddLocalizationString(pArrayString, "Controls");//23
		AddLocalizationString(pArrayString, "Walk Up");//24
		AddLocalizationString(pArrayString, "Walk Down");//25
		AddLocalizationString(pArrayString, "Walk Left");//26
		AddLocalizationString(pArrayString, "Walk Right");//27
		AddLocalizationString(pArrayString, "Fire Ball");//28
		AddLocalizationString(pArrayString, "Cone Fire");//29
		AddLocalizationString(pArrayString, "Fire Trap");//30
		AddLocalizationString(pArrayString, "Dash");//31
		AddLocalizationString(pArrayString, "Map");//32
		AddLocalizationString(pArrayString, "Attack");//33
		AddLocalizationString(pArrayString, "Use Spell");//34

		AddLocalizationString(pArrayString, "");//35
		AddLocalizationString(pArrayString, "A");//36
		AddLocalizationString(pArrayString, "B");//37
		AddLocalizationString(pArrayString, "C");//38
		AddLocalizationString(pArrayString, "D");//39
		AddLocalizationString(pArrayString, "E");//40
		AddLocalizationString(pArrayString, "F");//41
		AddLocalizationString(pArrayString, "G");//42
		AddLocalizationString(pArrayString, "H");//43
		AddLocalizationString(pArrayString, "I");//44
		AddLocalizationString(pArrayString, "J");//45
		AddLocalizationString(pArrayString, "K");//46
		AddLocalizationString(pArrayString, "L");//47
		AddLocalizationString(pArrayString, "M");//48
		AddLocalizationString(pArrayString, "N");//49
		AddLocalizationString(pArrayString, "O");//50
		AddLocalizationString(pArrayString, "P");//51
		AddLocalizationString(pArrayString, "Q");//52
		AddLocalizationString(pArrayString, "R");//53
		AddLocalizationString(pArrayString, "S");//54
		AddLocalizationString(pArrayString, "T");//55
		AddLocalizationString(pArrayString, "U");//56
		AddLocalizationString(pArrayString, "V");//57
		AddLocalizationString(pArrayString, "W");//58
		AddLocalizationString(pArrayString, "X");//59
		AddLocalizationString(pArrayString, "Y");//60
		AddLocalizationString(pArrayString, "Z");//61
		AddLocalizationString(pArrayString, "à");//62
		AddLocalizationString(pArrayString, "&");//63
		AddLocalizationString(pArrayString, "é");//64
		AddLocalizationString(pArrayString, "\"");//65
		AddLocalizationString(pArrayString, "\'");//66
		AddLocalizationString(pArrayString, "(");//67
		AddLocalizationString(pArrayString, "-");//68
		AddLocalizationString(pArrayString, "è");//69
		AddLocalizationString(pArrayString, "_");//70
		AddLocalizationString(pArrayString, "ç");//71
		AddLocalizationString(pArrayString, "Escape");//72
		AddLocalizationString(pArrayString, "LControl");//73
		AddLocalizationString(pArrayString, "LShift");//74
		AddLocalizationString(pArrayString, "LAlt");//75
		AddLocalizationString(pArrayString, "LSystem");//76
		AddLocalizationString(pArrayString, "RControl");//77
		AddLocalizationString(pArrayString, "RShift");//78
		AddLocalizationString(pArrayString, "RAlt");//79
		AddLocalizationString(pArrayString, "RSystem");//80
		AddLocalizationString(pArrayString, "Menu");//81
		AddLocalizationString(pArrayString, "[");//82
		AddLocalizationString(pArrayString, "]");//83
		AddLocalizationString(pArrayString, ";");//84
		AddLocalizationString(pArrayString, ",");//85
		AddLocalizationString(pArrayString, ".");//86
		AddLocalizationString(pArrayString, "'");//87
		AddLocalizationString(pArrayString, "/");//88
		AddLocalizationString(pArrayString, "\\");//89
		AddLocalizationString(pArrayString, "~");//90
		AddLocalizationString(pArrayString, "=");//91
		AddLocalizationString(pArrayString, "-");//92
		AddLocalizationString(pArrayString, "Space");//93
		AddLocalizationString(pArrayString, "Enter");//94
		AddLocalizationString(pArrayString, "Back");//95
		AddLocalizationString(pArrayString, "Tab");//96
		AddLocalizationString(pArrayString, "Page Up");//97
		AddLocalizationString(pArrayString, "Page Down");//98
		AddLocalizationString(pArrayString, "End");//99
		AddLocalizationString(pArrayString, "Home");//100
		AddLocalizationString(pArrayString, "Inser");//101
		AddLocalizationString(pArrayString, "Suppr");//102
		AddLocalizationString(pArrayString, "+");//103
		AddLocalizationString(pArrayString, "-");//104
		AddLocalizationString(pArrayString, "*");//105
		AddLocalizationString(pArrayString, "/");//106
		AddLocalizationString(pArrayString, "Left");//107
		AddLocalizationString(pArrayString, "Right");//108
		AddLocalizationString(pArrayString, "Up");//109
		AddLocalizationString(pArrayString, "Down");//110
		AddLocalizationString(pArrayString, "NumPad0");//111
		AddLocalizationString(pArrayString, "NumPad1");//112
		AddLocalizationString(pArrayString, "NumPad2");//113
		AddLocalizationString(pArrayString, "NumPad3");//114
		AddLocalizationString(pArrayString, "NumPad4");//115
		AddLocalizationString(pArrayString, "NumPad5");//116
		AddLocalizationString(pArrayString, "NumPad6");//117
		AddLocalizationString(pArrayString, "NumPad7");//118
		AddLocalizationString(pArrayString, "NumPad8");//119
		AddLocalizationString(pArrayString, "NumPad9");//120
		AddLocalizationString(pArrayString, "F1");//121
		AddLocalizationString(pArrayString, "F2");//122
		AddLocalizationString(pArrayString, "F3");//123
		AddLocalizationString(pArrayString, "F4");//124
		AddLocalizationString(pArrayString, "F5");//125
		AddLocalizationString(pArrayString, "F6");//126
		AddLocalizationString(pArrayString, "F7");//127
		AddLocalizationString(pArrayString, "F8");//128
		AddLocalizationString(pArrayString, "F9");//129
		AddLocalizationString(pArrayString, "F10");//130
		AddLocalizationString(pArrayString, "F11");//131
		AddLocalizationString(pArrayString, "F12");//132
		AddLocalizationString(pArrayString, "F13");//133
		AddLocalizationString(pArrayString, "F14");//134
		AddLocalizationString(pArrayString, "F15");//135
		AddLocalizationString(pArrayString, "Pause");//136


		AddLocalizationString(pArrayString, "");//137
		AddLocalizationString(pArrayString, "Mouse Left");//138
		AddLocalizationString(pArrayString, "Mouse Right");//139
		AddLocalizationString(pArrayString, "Mouse Middle");//140
		AddLocalizationString(pArrayString, "Mouse Button 1");//141
		AddLocalizationString(pArrayString, "Mouse Button 2");//142

		AddLocalizationString(pArrayString, "Dynamic Light"); //143

		AddLocalizationString(pArrayString, "Save"); //144

		AddLocalizationString(pArrayString, "Empty"); //145

		AddLocalizationString(pArrayString, "Player one"); //146
		AddLocalizationString(pArrayString, "Player two"); //147

		AddLocalizationString(pArrayString, "Keyboard"); //148
		AddLocalizationString(pArrayString, "Controller"); //149
		AddLocalizationString(pArrayString, "No device"); //150

		AddLocalizationString(pArrayString, "Shortcuts"); //151

		AddLocalizationString(pArrayString, "Disable"); //152
		AddLocalizationString(pArrayString, "Enable"); //153

		AddLocalizationString(pArrayString, "Master"); // 154
		AddLocalizationString(pArrayString, "Music"); // 155
		AddLocalizationString(pArrayString, "FX"); // 156

		AddLocalizationString(pArrayString, "New"); // 157
		AddLocalizationString(pArrayString, "Credits"); // 158

		AddLocalizationString(pArrayString, "Life's potions restore\nmort health."); // 159
		AddLocalizationString(pArrayString, "Invulnerability\nlasts longer."); // 160
		AddLocalizationString(pArrayString, "Maximum's life\nis increased."); // 161

		AddLocalizationString(pArrayString, "Mana's potions\nrestore more mana."); // 162
		AddLocalizationString(pArrayString, "Spells cost less mana."); // 163
		AddLocalizationString(pArrayString, "Maximum's mana\nis increased."); // 164

		AddLocalizationString(pArrayString, "The fire trap throw\nthree fireballs\nduring the explosion."); // 165
		AddLocalizationString(pArrayString, "Throw three fireballs\ninstead of one."); // 166
		AddLocalizationString(pArrayString, "All damages are\nincreased."); // 167

		AddLocalizationString(pArrayString, "Speed increased."); // 168
		AddLocalizationString(pArrayString, "Tenacity increased."); // 169
		AddLocalizationString(pArrayString, "Dash has two stacks."); // 170

		AddLocalizationString(pArrayString, "Not available."); // 171
		AddLocalizationString(pArrayString, "Improved to the\nmaximum."); // 172
		AddLocalizationString(pArrayString, "Cost"); // 173
		AddLocalizationString(pArrayString, "Your XP"); // 174

		AddLocalizationString(pArrayString, "One Player"); //175
		AddLocalizationString(pArrayString, "Two Players"); //176

		AddLocalizationString(pArrayString, "Slime"); //177
		AddLocalizationString(pArrayString, "Elemental"); //178

		AddLocalizationString(pArrayString, "Mode"); //179
		AddLocalizationString(pArrayString, "Dungeon"); //180
		AddLocalizationString(pArrayString, "Number of player"); //181

		AddLocalizationString(pArrayString, "Campaign"); //182
		AddLocalizationString(pArrayString, "Custom"); //183

		AddLocalizationString(pArrayString, "Dungeon\nSlime");//184
		AddLocalizationString(pArrayString, "Dungeon\nElemental");//185

		AddLocalizationString(pArrayString, "Introduction");//186
		AddLocalizationString(pArrayString, "Razar");//187

		break;
	}
}

void ChangeLocalization(LocalizationArray* pArrayString, const int pLanguage)
{
	for (int i = 0; i < pArrayString->Size; i++)
	{
		free(pArrayString->Array[i].String);
	}
	free(pArrayString->Array);

	InitializeLocalization(pArrayString, pLanguage);
}

void AddLocalizationString(LocalizationArray* pArrayString, const char* pString)
{
	if (pArrayString->Size >= NBR_TEXT)
	{
		printf("No space to allocate more text in localization\n");
		return;
	}
	Localization newLocalization;
	newLocalization.String = (char*)malloc(sizeof(char)*(strlen(pString) + 1));
	//Add new index to the new string
	newLocalization.Id = pArrayString->Size;
	strcpy(newLocalization.String, pString);
	pArrayString->Size++;

	pArrayString->Array[pArrayString->Size - 1] = newLocalization;
}

char* GetLocalizationString(const int pIndex)
{
	if (pIndex >= MainLocalization.Size)
	{
		return("Text Not Found");
	}
	else
	{
		return MainLocalization.Array[pIndex].String;
	}
}