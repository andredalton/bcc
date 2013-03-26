import java.util.Random;

public class Pessoa extends Thread {
	private int id;
	private int andarOrigem;
	private int andarDestino;
	private int modulo; // Módulo inteiro para a geração do tempo de espera para nascer
    private int espera; // Tempo de espera (em rodadas) para uma thread "nascer"
    private Random generator; // Gerador de número aleatórias
    private final Monitor monitorLocal; // Referência para o Monitor
    private boolean viva;
    private boolean dentroElevador;
	
    // Inicialização
	public Pessoa(int id, Monitor monitor) {
		this.id = id;
		this.viva = false;
		this.andarOrigem = 0;
		this.andarDestino = 0;
		this.dentroElevador = false;
		this.modulo = 4 * monitor.getNAndares();
		monitorLocal = monitor;
		this.generator = new Random();
		this.espera = 1 * this.generator.nextInt(this.modulo) + 1;
		start();
	}
	
	public boolean getViva() {
		return this.viva;
	}
	
	public boolean getDentroElevador() {
		return this.dentroElevador;
	}
	
	public int getAndarDestino() {
		return this.andarDestino;
	}
	
	public int getAndarOrigem() {
		return this.andarOrigem;
	}
	
	public void run() {
		while (true) {
			// Fica esperando para nascer
			while (this.espera > 0) {
				while (monitorLocal.getBarreira() != this.id || monitorLocal.terminouThreads() == true) {
					Main.busy();
				}
				try {
					monitorLocal.s.acquire();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				this.espera--;
				monitorLocal.incBarreira();
				monitorLocal.s.release();
			}
			// Nasce
			while (monitorLocal.getBarreira() != this.id || monitorLocal.terminouThreads() == true) {
				Main.busy();
			}
			try {
				monitorLocal.s.acquire();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			this.viva = true;
			this.andarOrigem = this.generator.nextInt(monitorLocal.getNAndares()) + 1;
			this.andarDestino = this.generator.nextInt(monitorLocal.getNAndares()) + 1;
			while (this.andarDestino == this.andarOrigem) { // Dessa forma, origem != destino
				this.andarDestino = this.generator.nextInt(monitorLocal.getNAndares()) + 1;
			}
			monitorLocal.incBarreira();
			monitorLocal.s.release();
			
			// Fica esperando para entrar no elevador
			while(true) {
				while (monitorLocal.getBarreira() != this.id || monitorLocal.terminouThreads() == true) {
					Main.busy();
				}
				try {
					monitorLocal.s.acquire();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				if (monitorLocal.tentaEntrarPessoa(this.andarOrigem) ) {
					break;
				}
				monitorLocal.incBarreira();
				monitorLocal.s.release();
			}
			this.dentroElevador = true;
			monitorLocal.entraPessoa();
			monitorLocal.incBarreira();
			monitorLocal.s.release();
			
			// Espera para sair do elevador
			while(true) {
				while (monitorLocal.getBarreira() != this.id || monitorLocal.terminouThreads() == true) {
					Main.busy();
				}
				try {
					monitorLocal.s.acquire();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				if (monitorLocal.tentaSairPessoa(this.andarDestino) ) {
					break;
				}
				monitorLocal.incBarreira();
				monitorLocal.s.release();
			}
			this.dentroElevador = false;
			monitorLocal.saiPessoa();
			this.viva = false;
			this.espera = 1 * this.generator.nextInt(this.modulo) + 1;
			monitorLocal.incBarreira();
			monitorLocal.s.release();
			
		}
	}
}
