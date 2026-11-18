import ghidra.app.script.GhidraScript;
import ghidra.app.plugin.processors.sleigh.SleighDebugLogger;
import ghidra.program.model.address.Address;

public class DumpSleighDebug20843 extends GhidraScript {
    @Override
    public void run() throws Exception {
        Address addr = currentProgram.getAddressFactory().getAddress("00020843");
        SleighDebugLogger logger = new SleighDebugLogger(currentProgram, addr, SleighDebugLogger.SleighDebugMode.VERBOSE);
        println("=== SleighDebugLogger output for 0x20843 ===");
        println(logger.toString());
        println("=== END ===");
    }
}
