// @ : vpotin
// but : programmation d'une serrure électronique de type casier de piscine

#include <Keypad.h>
#include <LiquidCrystal.h>         
#include <Servo.h>

        // -- Déclaration des entrées/sorties et variables utilisées -- //


LiquidCrystal lcd(14, 15, 16, 17, 18, 19); // Connexions pour l'écran d'affichage LCD

const byte ROWS = 4; //4 rangées
const byte COLS = 4; //4 colonnes

char hexakeys[ROWS][COLS] = { // Définition du pad numérique utilisé
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = { 9, 8, 7, 6 }; // Connexion du pad à l'Arduino (les rangées) 
byte colPins[COLS] = {5, 4, 3, 2}; // Connexion du pad à l'Arduino (les colonnes)

Servo servomoteur; // Déclaration du servomoteur

int redPin = 11; // Connexion pour la led rouge de fermeture
int greenPin = 12; // Connexion pour la led verte d'ouverture

Keypad membraneKeypad = Keypad( makeKeymap(hexakeys), rowPins, colPins, ROWS, COLS);  // Déclaration du pad numérique


const int tailleMessageMax = 5;
char password[tailleMessageMax + 1] =   {'0','0','0','0','0','\0'};
char entree[tailleMessageMax + 1] = {'0','0','0','0','0','\0'};



        // -- Affichage des données tapées par l'utilisateur -- //

void affichelaReponseEnCours(char message[6])
{
  lcd.clear();
  lcd.print(message);
}


         // -- Traitement des entrées sur clavier -- //

void lireEntree(char message[6])
{
  char key;
  unsigned int indexMessage = 0; 
  boolean saisieEnCours = true;

  message[0] = '\0';

  while(saisieEnCours) {
    key = membraneKeypad.getKey();
    switch (key) {
      case NO_KEY :
      break;

      case '*': // effacer un caractère
      case 'A': // effacer un caractère
      case 'B': // effacer un caractère
      case 'C': // effacer un caractère
      case 'D': // effacer un caractère
         if (indexMessage > 0) {
          message[--indexMessage] = '\0';
          affichelaReponseEnCours(message);
         }
      break;
      case '#': // valider l'entrée
          saisieEnCours = false;
          break;

      default:
        message[indexMessage++] = key;
        message[indexMessage] = '\0'; 
        affichelaReponseEnCours(message);
        if (indexMessage >= tailleMessageMax - 1) {
          saisieEnCours = false; 
        }
        break;
       
    }
  }
 
}


     
      // -- Contrôle du servomoteur -- //



void setLocked(int locked)
{
  if (locked)
  {
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    delay(1000);
    servomoteur.attach(10);
    servomoteur.write(90);
    
  }
  else
  {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    delay(1000);
    servomoteur.attach(10);
    servomoteur.write(0);
    
  }
}

void(* resetFunc) (void) = 0;



    // -- Séquence de Fonctionnement -- //


void setup() {
  Serial.begin(115200);
  while(!Serial);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  lcd.begin(16, 2);
  setLocked(false);
  lcd.print("Welcome");
  delay(2000);
  lcd.clear();
  delay(2000);
  lcd.print("Enter Password :");
  lireEntree(password);
  setLocked(true);
  delay(1000);
  lcd.clear();
  delay(1000);
  lcd.print("Good Shopping");
  delay(3000);
  lcd.clear();
  delay(1000);
  
  
}

void loop() {
  String pass = String(password);
  lcd.print("Enter Password:");
  lireEntree(entree);
  String ent = String(entree);
  lcd.clear();
  delay(1000);
  if (ent==pass or ent=="4758") {     //on introduit un code de sécurité et universel 4758
      lcd.clear();
      lcd.print("approved");
      setLocked(false);
      delay(500);
      lcd.clear();
      delay(500);
      lcd.print("See you soon");
      delay(1000);
      lcd.clear();
      resetFunc();    }
  else {
      lcd.clear();
      lcd.print("wrong password");
      delay(3000);
      lcd.clear();
      delay(1000);
  }
}
  
  
 
  
  
     
  
  

  
  
     
  
  
