/** TODO: make this work without dependencies
 * 
 */
#include <string>
#include <filesystem>
#include <./CLI11/CLI.hpp>

using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::is_directory;

#define MIN_GPU_CNT 1
#define MAX_GPU_CNT 4

int main(int argc, char* argv[]) {
    /** Define Variables **/
    string datapath = get_current_dir_name();   /** Path to data directory */
    datapath.append("/data/");
    string dataset = "Set1";                     /** Default dataset */
    string filter = "None";                     /** Filter for data */
    int queue_size = 1;                         /** Number of GPUs to use */
    bool batch = false;                         /** if true, then run lsml on all the data directories in data folder */
    bool toggle_sim = false;                    /** if true, then simulate data rather than import real data */
    bool toggle_mc = false;                     /** if true, then perform monte carlo analysis */

    /**** START CLI INPUT ****/
    CLI::App app("LSML CLI");
    app.add_option("-p, --path", dataset, "Path to data directory [Disabled if running batch]")
        ->default_val("OG");
        // ->check(CLI::detail::Datapath_Validator());
    app.add_option("-f, --filter", filter, "Filter for data")
        ->default_val("None");
    app.add_option("-d, --devices", queue_size, "Amount of GPUs to use")
        ->default_val(1);
    app.add_flag("-b, --batch", batch, "Run all the datasets in the data folder")
        ->default_val(false);
    app.add_flag("-s, --simulate", toggle_sim, "Use simulated data rather than json")
        ->default_val(false);
    app.add_flag("-m, --montecarlo", toggle_mc, "Use Monte Carlo analysis")
        ->default_val(false);
    try { app.parse(argc, argv); }
    catch ( const CLI::CallForHelp &e ) { exit( app.exit(e) ); } // Outputs Help options if -h is an argument

    /** Validate Filter Input **/
    if (filter == "W") { filter = "Whitelist"; }
    else if (filter == "B") { filter = "Blacklist"; }
    else if (filter == "M") { filter = "Manual"; }
    else if (filter == "N") { filter = "None"; }
    else {
        filter = "None";
        printf("lsml: Invalid Filter! Using Default... [Default='None']\n\n");
    }
    printf("lsml: Using Filter: %s\n\n", filter.c_str()); 
    
    /** Validate GPU Count **/
    if (queue_size >= MIN_GPU_CNT && queue_size <= MAX_GPU_CNT) {
        printf("lsml: Using GPU count: %d\n", queue_size);
    } else {
        queue_size = 1;
        printf("lsml: Invalid GPU amount! Using Default... [Default=%d]\n", 1);
    }
    /** Validate Datapath Input **/
    if (batch) {
        DIR *dir;
        struct dirent *entry = nullptr;
        if ((dir = opendir(datapath.c_str())) == NULL) { perror("lsml: opendir() error"); }
        else {
            while ((entry = readdir(dir))) {
                dataset = entry->d_name;
                if (dataset != "." && dataset != ".." && dataset != "filter.json" && dataset != ".data_Truth.json" && dataset != "output" && dataset != "Dalton") {
                    printf("lsml-batch: Current Dataset: %s\n", dataset.c_str());

                    // double lsml_time = magma_sync_wtime(NULL);
                    // LoadingProcessing(datapath, dataset, filter, queue_size, batch, toggle_sim, toggle_mc);
                    // lsml_time = magma_sync_wtime(NULL) - lsml_time;
                    printf("\nlsml: Time to run %s: %f seconds\n\n", dataset.c_str(), lsml_time);
                }
            }
            closedir(dir);
        }
    }
    /**
     * FIXME: Using the "path" datatype causes a major segfault on destruction in gcc versions <9
     * Meaning that this code will not work with the following validity check for now.
     * Check if the selected directory exists and is valid
     */
    // path datatest = datapath + dataset;
    // else if (is_directory(datatest)) {
    else if (true) {
        printf("lsml: Using Dataset: %s\n", dataset.c_str());

        double lsml_time = magma_sync_wtime(NULL);
        LoadingProcessing(datapath, dataset, filter, queue_size, batch, toggle_sim, toggle_mc);
        lsml_time = magma_sync_wtime(NULL) - lsml_time;
        printf("lsml: Time to run whole program: %f seconds\n", lsml_time);
    } else {
        printf("lsml: Invalid Dataset! Using Default... [Default=OG/]\n\n");
        printf("lsml: NOTICE: This program must be run from the project root directory\n        If the dataset exists and is in the right place, try changing\n        the directory.\n\n");
        exit(1);
    }
    /***** END CLI INPUT *****/

    return 0;
}