// Exports, for every NAMED (non-generic: not DAT_*, not TABLE_2D_*/TABLE_3D_*/
// SCALING_TABLE_* auto-scraper placeholders) data symbol in the CURRENT program,
// the set of calling function names that reference it (via Ghidra's own reference
// database -- populated by FixBankOffsetReferences.java and
// FixImmediatePushReferences.java).
//
// Intended use: run on EVO5 (which has real XML-imported table names AND now has
// real xrefs from the two Fix*References scripts) to produce a
// function-name -> table-name map. Since RVR and EVO5 share function names via the
// rvr_function_signatures.csv fuzzy match, this map can be used to infer names for
// RVR's still-unnamed (DAT_*/TABLE_*/SCALING_TABLE_*) data: look up which named EVO5
// table each RVR caller function's counterpart points to, and if there's a clean
// 1:1 match, that's very likely the same real-world calibration table.
//
// Output: CSV to the project root, one row per (table, caller) pair:
//   table_addr,table_name,caller_addr,caller_name
// Only includes callers whose name does NOT start with FUN_ (i.e. only rows useful
// for cross-referencing, since a FUN_* caller in EVO5 can't be matched to anything
// in RVR by name anyway).
//
// @category H8539F

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.DataIterator;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolTable;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.LinkedHashSet;
import java.util.Set;

public class ExportNamedTableCallers extends GhidraScript {
    @Override
    public void run() throws Exception {
        Listing listing = currentProgram.getListing();
        SymbolTable symTab = currentProgram.getSymbolTable();
        ReferenceManager refMan = currentProgram.getReferenceManager();

        String outPath = "C:\\Users\\j.brophy.CORKILLSYSTEMS\\github\\Ghidra-H8-Processor\\named_table_callers.csv";
        File outFile = new File(outPath);
        PrintWriter pw = new PrintWriter(new FileWriter(outFile, false));
        pw.println("table_addr,table_name,caller_addr,caller_name,caller_entry");

        int tablesWritten = 0;
        int rowsWritten = 0;

        DataIterator dataIt = listing.getDefinedData(true);
        while (dataIt.hasNext()) {
            Data d = dataIt.next();
            Address a = d.getAddress();

            Symbol sym = symTab.getPrimarySymbol(a);
            if (sym == null) continue;
            String name = sym.getName();

            // Skip generic/placeholder names -- only interested in real, meaningful names.
            if (name.startsWith("DAT_")) continue;
            if (name.startsWith("TABLE_2D_")) continue;
            if (name.startsWith("TABLE_3D_")) continue;
            if (name.startsWith("SCALING_TABLE_")) continue;
            if (name.startsWith("BYTE_ARRAY_")) continue;

            ReferenceIterator refs = refMan.getReferencesTo(a);
            Set<String> callerRows = new LinkedHashSet<>();
            boolean any = false;
            while (refs.hasNext()) {
                Reference r = refs.next();
                Address fromAddr = r.getFromAddress();
                Function caller = listing.getFunctionContaining(fromAddr);
                String callerName = (caller != null) ? caller.getName() : "UNKNOWN";
                String callerEntry = (caller != null) ? caller.getEntryPoint().toString() : "00000000";
                // Emit regardless of whether the caller is named -- RVR-side lookup can
                // match on caller ENTRY ADDRESS even when the name is still FUN_*,
                // since function start addresses are far more likely to still align
                // between the two ROMs than the fuzzy-matcher's name coverage is complete.
                String row = String.format("%08x,%s,%08x,%s,%08x", a.getOffset(), name, fromAddr.getOffset(), callerName, java.lang.Long.parseLong(callerEntry, 16));
                if (callerRows.add(row)) {
                    any = true;
                }
            }
            if (any) {
                for (String row : callerRows) {
                    pw.println(row);
                    rowsWritten++;
                }
                tablesWritten++;
            }
        }

        pw.close();
        println("Wrote " + rowsWritten + " (table, caller) rows for " + tablesWritten + " named tables to:");
        println(outPath);
    }
}
