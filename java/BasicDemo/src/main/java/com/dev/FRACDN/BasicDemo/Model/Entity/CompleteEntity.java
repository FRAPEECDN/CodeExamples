package com.dev.FRACDN.BasicDemo.Model.Entity;

import jakarta.persistence.*;
import lombok.*;
import org.hibernate.annotations.ColumnDefault;
import org.hibernate.annotations.JdbcTypeCode;
import org.hibernate.annotations.UuidGenerator;

import java.util.UUID;

@Builder
@Getter
@Setter
@Entity
@Table(name = "complete")
@NoArgsConstructor
@AllArgsConstructor
public class CompleteEntity {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private Long _id;

    @Column(name = "publicId", updatable = false, nullable = false, columnDefinition = "VARCHAR(36)")
    @UuidGenerator(style = UuidGenerator.Style.TIME)
    @ColumnDefault("random_uuid()")
    @JdbcTypeCode(java.sql.Types.VARCHAR)
    private String publicId;

    @Column(length = 10, nullable = false)
    private String description;
}
