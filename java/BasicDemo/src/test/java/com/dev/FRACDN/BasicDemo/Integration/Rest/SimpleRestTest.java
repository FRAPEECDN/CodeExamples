package com.dev.FRACDN.BasicDemo.Integration.Rest;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.boot.test.web.client.TestRestTemplate;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpMethod;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;

import static org.assertj.core.api.Assertions.assertThat;

/**
 * Test the Simple Controller specifically
 */
@SpringBootTest(webEnvironment = SpringBootTest.WebEnvironment.RANDOM_PORT)
public class SimpleRestTest {

    private static final String PATH = "/simple";
    private static final String BODY = "abc";


    @Autowired
    private TestRestTemplate template;

    @Test
    public void getAllSimpleTest() {
        ResponseEntity<String> response = template.getForEntity(PATH, String.class);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isNotBlank();
        assertThat(response.getHeaders()).isNotEmpty();
    }
    @Test
    public void getSingleSimpleTest() {
        ResponseEntity<String> response = template.getForEntity(PATH + "/{id}", String.class, 0);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isNotBlank();
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void createSimpleTest() {
        ResponseEntity<String> response = template.postForEntity(PATH, BODY, String.class);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.CREATED);
        assertThat(response.getBody()).isNotBlank();
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void updateSimpleTest() {
        HttpEntity<String> requestEntity = new HttpEntity<>(BODY);
        ResponseEntity<String> response = template.exchange(PATH + "/{id}", HttpMethod.PUT, requestEntity, String.class, 0);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isNotBlank();
        assertThat(response.getHeaders()).isNotEmpty();
    }

    @Test
    public void deleteSimpleTest() {
        ResponseEntity<Void> response = template.exchange(PATH + "/{id}", HttpMethod.DELETE,null, Void.class,0);
        assertThat(response.getStatusCode()).isEqualTo(HttpStatus.OK);
        assertThat(response.getBody()).isNull();
        assertThat(response.getHeaders()).isNotEmpty();
    }
}
