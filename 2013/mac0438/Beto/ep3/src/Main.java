import java.util.Scanner;

public class Main extends Thread {
	// Função principal
	public static void main (String[] args) throws InterruptedException {
		int i;
		int nAndares = Integer.parseInt(args[0]); // Número de andares dado na entrada
		int capacidade = Integer.parseInt(args[1]); // Capacidade do elevador dado na entrada
		int nThreads = 20 * nAndares + capacidade; // Número máximo de pessoas que podem existir simultaneamente
		
		// Cria o Ascensorista (Monitor) -> 1 thread
		Monitor monitor = new Monitor(nAndares, capacidade, nThreads);
		// Cria as Pessoas (Threads) -> (nThreads - 1) threads
		Pessoa[] pessoas = new Pessoa[nThreads];
		for (i = 0; i < nThreads - 1; i++) {
			pessoas[i] = new Pessoa(i + 1, monitor);
		}
		
		// Laço principal (infinito)
		while(true) {
			while(! monitor.terminouThreads()) { // Espera todas as threads de Pessoas chegarem na barreira
				busy();
			}
			
			// Impressão dos dados
            System.out.println("\nAndar Atual: " + monitor.getAndarAtual());
            System.out.println("Entraram: " + monitor.getEntraramRodada());
            System.out.println("Saíram: " + monitor.getSairamRodada());
            System.out.println("\nNo elevador: " + monitor.getNPessoasElevador());
            for (i = 0; i < nThreads - 1; i++) {
            	if (pessoas[i].getDentroElevador()) {
            		System.out.println("Origem: " + pessoas[i].getAndarOrigem() + " -> Destino: " + pessoas[i].getAndarDestino());
            	}
            }
            System.out.println("\nEsperando:");
            for (i = 0; i < nThreads - 1; i++) {
            	if (pessoas[i].getViva() && ! pessoas[i].getDentroElevador()) {
            		System.out.println("Origem: " + pessoas[i].getAndarOrigem() + " -> Destino: " + pessoas[i].getAndarDestino());
            	}
            }
			
            // Espera a próxima iteração
			hitEnter();
			// Movimenta o elevador
			monitor.movimenta();
		}
    }

	public static void hitEnter(){
		Scanner keyboard = new Scanner(System.in);
		keyboard.nextLine();
	}

	// Sleep
	public static void busy() {
        try {
          sleep(5);
        } 
        catch (InterruptedException e)
        {
        }
    }
}