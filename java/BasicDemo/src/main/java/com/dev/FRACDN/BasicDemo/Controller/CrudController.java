package com.dev.FRACDN.BasicDemo.Controller;

import com.dev.FRACDN.BasicDemo.Exceptions.NotFoundException;
import com.google.common.base.Preconditions;
import lombok.RequiredArgsConstructor;
import org.springframework.http.HttpStatus;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping(path = "/crud")
@RequiredArgsConstructor
public class CrudController {

    public static final List<String> RETURN_LIST = List.of("a for apple", " b for bananas");
    public static final String ONE_OBJECT = "a for apple";
    public static final Integer RESULT_CREATE = 1;

    @GetMapping
    public List<String> findAll() {
        return RETURN_LIST;
    }

    @GetMapping(value = "/{id}")
    public String findById(@PathVariable("id") int id) {
        return ONE_OBJECT;
    }

    @PostMapping
    @ResponseStatus(HttpStatus.CREATED)
    public int create(@RequestBody String resource) {
        Preconditions.checkNotNull(resource);
        return RESULT_CREATE;
    }

    @PutMapping(value = "/{id}")
    @ResponseStatus(HttpStatus.OK)
    public String update(@PathVariable("id") int id, @RequestBody String resource)  {
        return ONE_OBJECT;
    }

    @DeleteMapping(value = "/{id}")
    @ResponseStatus(HttpStatus.OK)
    public void delete(@PathVariable("id") int id) {
    }
}
