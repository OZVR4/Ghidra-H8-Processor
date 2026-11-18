// @category H8Processor
import ghidra.app.script.GhidraScript;

public class TestBundleLoad extends GhidraScript {
    @Override
    public void run() throws Exception {
        println("TEST BUNDLE LOAD OK. currentProgram=" + (currentProgram != null ? currentProgram.getName() : "null"));
    }
}
