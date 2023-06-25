package com.dev.FRACDN.BasicDemo.Model.Entity;

import com.dev.FRACDN.BasicDemo.Model.BasicEnum;
import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.JdbcTypeCode;
import org.hibernate.annotations.UuidGenerator;

import java.util.UUID;


/**
 * Basic Object Entity that will be stored in database
 */
@Builder
@Getter
@Setter
@Entity
@Table(name = "BASIC")
@NoArgsConstructor
@AllArgsConstructor
public class BasicEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long _id;

    @Column(name = "publicId", updatable = false, nullable = false, columnDefinition = "VARCHAR(36)")
    @UuidGenerator(style = UuidGenerator.Style.TIME)
    @JdbcTypeCode(java.sql.Types.VARCHAR)
    private UUID publicId;

    @Column(length = 64, nullable = false)
    private String description;

    @Column(columnDefinition = "int")
    private Integer number;

    @Column(nullable = false)
    private Boolean choose;

    @Column(nullable = false)
    private BasicEnum basicEnum;

}
