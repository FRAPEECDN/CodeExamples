package com.dev.FRACDN.BasicDemo.Service;

import com.google.common.base.Preconditions;
import lombok.RequiredArgsConstructor;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.RequestBody;

import javax.annotation.Nullable;
import java.util.ArrayList;
import java.util.List;

/**
 * Service level of simple controller
 */
@Service
@RequiredArgsConstructor
public class SimpleService {

    private final List<String> respositoryList = new ArrayList<>();

    /**
     * Return a list of string as service request
     * @return list of Strings
     */
    public List<String> getAll() {
        return this.respositoryList;
    }

    /**
     * return a specific string
     * @param id id (index of string)
     * @return a stored string
     */
    public String findById(int id) {
        Preconditions.checkElementIndex(id, respositoryList.size());
        return this.respositoryList.get(id);
    }

    /**
     * Create a new string aka add it
     * @param value value as string being added
     * @return index where added (end of list)
     */
    public int create(@Nullable String value) {
        Preconditions.checkNotNull(value);
        this.respositoryList.add(value);
        return this.respositoryList.size() - 1;
    }

    /**
     * Update a string to new value
     * @param id id (index of string)
     * @param newValue new value to replace old string
     * @return updated string
     */
    public String update(int id, @RequestBody String newValue)  {
        Preconditions.checkElementIndex(id, respositoryList.size());
        Preconditions.checkNotNull(newValue);
        Preconditions.checkNotNull(this.respositoryList.get(id));
        return this.respositoryList.set(id, newValue);
    }

    /**
     * Delete a string
     * @param id id (index of string)
     */
    public void delete(int id) {
        Preconditions.checkElementIndex(id, respositoryList.size());
        this.respositoryList.remove(id);
    }

    /**
     * Testing helper -> reset internal list
     */
    void resetRepositoryList() {
        this.respositoryList.clear();
    }

    /**
     * Testing helper -> populate internal list
     * @param input input list to add for population;
     */
    void setInternalStrings(List<String> input) {
        this.respositoryList.addAll(input);
    }
}
