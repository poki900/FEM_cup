# MES symulacja transportu ciepła w ceramicznynm kubku:
1. Opis problemu: 
    -  Program zakłada zasymulowanie ochładzania się cieczy (wody o temperaturze 100oC) w kubku ceramicznym.

        ![alt text](image-1.png)
    -  W celu porównania wyników został przeprowadzony pomiar rzeczywistych danych. Aby w późniejszym etapie móc porównywać tylko i wyłącznie transport ciepła poprzez ścianki boczne kubka problem uproszczono zakładając izolację od góry i od dołu kubka.

        ![alt text](image.png)
    -  Prowadzony był trzykrotny pomiar temperatury w odstępach co 10s przez pierwszą 1min. Następnie co 1min aż do 6 min i dalej co 2 min aż do 40 min. Pomiar temperatury wody był prowadzony sondą. Planowany pomiar temperatury kubka nie był prowadzony ze względu na dużą niedokładność termometru emisyjnego.
    -  Dokładność sondy to ±0,1oC.
    -  W trakcie pomiarów zaobserwowano nieprzewidziany problem w postaci konwekcyjnej wymiany ciepła pomiędzy kubkiem a wodą w pierwszych 50 s co skutkowało gwałtownym spadkiem temperatury płynu. W programie uwzględniono tą konwekcję.
    -  Dodatkowo w celu porównania przeprowadzono 2 symulacje: w pierwszej kubek miał temperaturę pokojową, natomiast w drugiej został podgrzany do temperatury wody. W pierwszym przypadku występowała konwekcja pomiędzy kubkiem a wodą, natomiast w drugiej nie występowała – dominującą metodą wymiany ciepła stało się przewodzenie.
    -  Do symulacji użyto napisanego w całości samodzielnie programu na podstawie algorytmu metody elementów skończonych. 
2. Symulacja:
    -  W celu wygenerowania siatki został napisany program generator.cpp. Program ten w funkcji main przyjmuje od użytkownika parametry ogólne i parametry kubka oraz wpisuje parametry wody. W funkcji licz() program wylicza odpowiednie współrzędne wierzchołków.
    -  Współrzędne są wyliczane według schematu:

        ![alt text](image-2.png)
    -  Na schemacie zaprezentowano przykładowy element oraz metodę rozwiązania problemu środka koła (jak podzielić w ostatnim etapie wierzchołki tak aby powstały czworokąty – liczba wierzchołków na pojedynczym okręgu musi być podzielna przez 2). Dwa pierwsze rzędy wierzchołków to punkty symulujące kubek, pozostałe symulują wodę.
    -  Przykład wygenerowanego pliku to plik generated_example.txt
    -  Oznaczenie danych w generowanym pliku:
            
        -  SimulationTime - czas symulacji
        -  SimulationStepTime  - czas kroku czasowego
        -  Conductivity - przewodność cieplna dla kubka
        -  Alfa - parametr alfa konwekcyjnej wymiany ciepła pomiędzy kubkiem a powietrzem
        -  Tot - temperatura otoczenia
        -  InitialTemp - temperatura wstępna kubka
        -  Density - gęstość kubka
        -  SpecificHeat - ciepło właściwe dla kubka
        -  Nodes number - ilość wierzchołków siatki
        -  Elements number - ilość elementów siatki
        -  *Node - początek listy wierzchołków wraz z ich współrzędnymi w układzie dwuwymiarowym
        -  *Element - początek listy elementów wraz z id odpowiadający im punktom
        -  *BC - lista wierzchołków znajdjących się na granicy kubek-powietrze bądź kubek-woda
        -  Conductivity2 - przewodność cieplna wody
        -  InitialTemp2 - temperatura wstępna wody
        -  Density2 - gęstość wody
        -  SpecificHeat2 - ciepło właściwe wody
        -  NoWaterNr - ilość węzłów siatki symlujących kubek
        -  Alfa2 - parametr alfa konwekcyjnej wymiany ciepła pomiędzy wodą a kubkiem
    -  Główny plik symulacji to program2.cpp. Plik uklad.cpp odpowiada za rozwiązanie układu równań metodą eliminacji Gaussa.
    -  Dodatkowo zaimplementowano symulację konwekcji pomiędzy wodą a kubkiem w pierwszym kroku (do 50s). Wynika ona z dużej różnicy temperatur pomiędzy ścianką a płynem, w późniejszym etapie temperatury się wyrównują i dominującą metodą wymiany ciepła pomiędzy wodą, a kubkiem staje się przewodzenie, gdyż różnice są za małe aby zaistniała konwekcja. Przez całą symulację zachodzi konwekcja pomiędzy powietrzem a kubkiem.
    -  W celu zasymulowania mieszania się płynu zastosowano po każdym kroku wyliczanie średniej ze wszystkich punktów siatki które symulują wodę i wpisanie tej średniej do wartości temperatur odpowiadającej tym punktom.
    