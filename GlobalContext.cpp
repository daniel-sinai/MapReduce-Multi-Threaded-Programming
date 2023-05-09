#include "GlobalContext.h"

GlobalContext::GlobalContext (const MapReduceClient &client,
                              const InputVec &inputVec, OutputVec &outputVec,
                              int multiThreadLevel)
{
    this->input_vec = inputVec;
    this->output_vec = outputVec;
    this->client = &client;
    this->pairs_number = inputVec.size ();
    this->set_stage(UNDEFINED_STAGE);
    this->multi_thread_level = multiThreadLevel;
    this->threads_barrier = new(std::nothrow) Barrier(multiThreadLevel);
    if (this->threads_barrier == nullptr)
    {
        fprintf(stderr, MEMORY_ALLOC_FAILED);
        exit(EXIT_ERROR_CODE);
    }
}

void GlobalContext::job_manager () {
    // map phase till end list
    // wait for all map phase
    // wait for shuffle
    // reduce phase till end list
    // terminate
}

void GlobalContext::map_manager(ThreadContext* tc) {
    uint32_t curr_id = tc->increment_next_pair_index();
     while (curr_id < get_pairs_number ()) {
        tc->set_curr_vector_id(curr_id);
        InputPair key_value = this->input_vec[curr_id];
        this->client->map(key_value.first, key_value.second, tc);
        curr_id = tc->increment_next_pair_index();
     }

}

void GlobalContext::sort_mid_vector(ThreadContext *tc) {
    std::sort(tc->get_vector()->begin(), tc->get_vector()->end(), GlobalContext::compare_k2);
}

void GlobalContext::shuffle_manager(ThreadContext* tc) {
    K2* curr_max_k2 = find_max_k2_from_threads_vectors();
    while (curr_max_k2 != nullptr) {
        std::vector<IntermediatePair>* curr_key_vector = new std::vector<IntermediatePair> ();
        for (auto curr_tc : this->contexts) {
            std::vector<IntermediatePair>* curr_thread_vector = curr_tc->get_vector();
            if (curr_thread_vector->empty()) { continue; }
            K2* curr_thread_max_key = curr_thread_vector->back().first;
            while (is_intermediary_keys_equal(curr_thread_max_key, curr_max_k2)) {
                curr_key_vector->push_back(curr_thread_vector->back ());
                tc->increment_progress_counter();
            }
        }
        this->shuffled_vectors.push_back(curr_key_vector);
        curr_max_k2 = find_max_k2_from_threads_vectors ();
    }
}

K2 *GlobalContext::find_max_k2_from_threads_vectors() {
    K2* cur_max = nullptr;
    for (auto tc : this->contexts)
    {
        if (tc->get_vector()->empty()) { continue; }
        K2* cur_k2 = tc->get_vector()->back().first;
        if (cur_max == nullptr)
        {
            cur_max = cur_k2;
        }
        else if (*cur_max < *cur_k2)
        {
            cur_max = cur_k2;
        }
    }
    return cur_max;
}

bool GlobalContext::is_intermediary_keys_equal(K2 *key1, K2 *key2) {
    return (not(*key1 < *key2)) and (not(*key2 < *key1));
}


int GlobalContext::compare_k2(const IntermediatePair& p1, const IntermediatePair& p2) {
    if (*(p1.first) < *(p2.first)) {
        return -1;
    }
    else if (*(p2.first) < *(p1.first)) {
        return 1;
    }
    else {
        return 0;
    }
}

void GlobalContext::reset_counters() {
    this->progress_counter.exchange(0);
    this->next_pair_index.exchange(0);
}
