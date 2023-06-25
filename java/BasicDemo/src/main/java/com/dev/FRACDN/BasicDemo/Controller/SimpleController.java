package com.dev.FRACDN.BasicDemo.Controller;

import com.dev.FRACDN.BasicDemo.Service.SimpleService;
import lombok.RequiredArgsConstructor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;

/**
 * Simple controller for some very basic tests being run
 */
@RestController
@RequestMapping(path = "/simple")
@RequiredArgsConstructor
public class SimpleController {

    @Autowired
    private final SimpleService simpleService;


    @GetMapping
    public List<String> findAll() {
        return this.simpleService.getAll();
    }

    @GetMapping(value = "/{id}")
    public String findById(@PathVariable("id") int id) {
        return this.simpleService.findById(id);
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public int create(@RequestBody String value) {
        return this.simpleService.create(value);
    }

    @PutMapping(value = "/{id}")
    @ResponseStatus(HttpStatus.OK)
    public String update(@PathVariable("id") int id, @RequestBody String newValue)  {
        return this.simpleService.update(id, newValue);
    }

    @DeleteMapping(value = "/{id}")
    @ResponseStatus(HttpStatus.OK)
    public void delete(@PathVariable("id") int id) {
        this.simpleService.delete(id);
    }
}
